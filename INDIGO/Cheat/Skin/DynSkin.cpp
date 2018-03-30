#include "DynSkin.h"

namespace DynSkin
{
	namespace File
	{
		CFile::CFile( void ) : _instance( this ) {}
		CFile::~CFile( void ) {}

		bool CFile::Load( const string& path , bool isunicode /* = false*/ )
		{
			Release();

			if ( GetFileAttributesA( path.c_str() ) == 0xFFFFFFFF )//sanity check
				return false;

			_path = path;

			ifstream ifs( _path , ios::binary | ios::ate );
			auto pos = ifs.tellg();

			auto tmp = vecFile( szPos( pos ) );

			ifs.seekg( 0 , ios::beg );
			ifs.read( &tmp.at( 0 ) , pos );

			if ( isunicode )
			{
				for ( unsigned int i = 0; i < pos; i += 2 )
				{
					_file.push_back( tmp.at( i ) );
				}
			}
			else
			{
				_file = tmp;
			}

			return Parse();
		}

		void CFile::Release( void )
		{
			if ( !_file.empty() )
			{
				_file.clear();
			}

			_path.clear();
		}

		szPos CFile::FindFirstOf( const string& szSearch , szPos start , szPos end )
		{
			auto isInverse = bool( start > end );

			if ( _file.size() <= ( isInverse ? start : end ) )
				return _file.size();

			if ( isInverse )
			{
				for ( auto r = start; r > end; --r )
				{
					if ( CompareBytes( static_cast<char*>( &_file.at( r ) ) , const_cast<char*>( &szSearch.at( 0 ) ) ) )
						return r;
				}
			}
			else
			{
				for ( auto r = start; r < end; ++r )
				{
					if ( CompareBytes( static_cast<char*>( &_file.at( r ) ) , const_cast<char*>( &szSearch.at( 0 ) ) ) )
						return r;
				}
			}

			return _file.size();
		}

		vecPos CFile::FindAllOf( const string& szSearch , szPos start , szPos end , vecPos pos /*= vecPos( )*/ )
		{
			auto isInverse = bool( start > end );

			if ( _file.size() <= ( isInverse ? start : end ) )
				return pos;

			if ( isInverse )
			{
				for ( auto r = start; r > end; --r )
				{
					if ( CompareBytes( static_cast<char*>( &_file.at( r ) ) , const_cast<char*>( &szSearch.at( 0 ) ) ) )
						pos.push_back( r );
				}
			}
			else
			{
				for ( auto r = start; r < end; ++r )
				{
					if ( CompareBytes( static_cast<char*>( &_file.at( r ) ) , const_cast<char*>( &szSearch.at( 0 ) ) ) )
						pos.push_back( r );
				}
			}

			return pos;
		}

		string CFile::GetStringAt( szPos start , szSize length )
		{
			if ( _file.size() <= ( start + length ) )
				return string( "" );

			return string( static_cast<char*>( &_file.at( start ) ) , length );
		}

		string CFile::GetStringBetween( szPos start , szPos end )
		{
			auto isInverse = bool( start > end );

			if ( _file.size() <= ( isInverse ? start : end ) )
				return string( "" );

			auto rstart = isInverse ? end : start;
			auto rend = isInverse ? start : end;

			return string( static_cast<char*>( &_file.at( rstart ) ) , szPos( rend - rstart ) );
		}

		namespace Valve
		{
			CLevel::CLevel( szPos& start , CFile* file ) :
				_start( start ) ,
				_end( 0 )
			{
				auto size = file->GetFileSize();

				szPos vpos[4] = { 0 };
				unsigned int qcount = 0;

				for ( ++start; start < size; ++start )
				{
					const auto& p = file->at( start );
					if (file->GetStringAt(start, 37) == "SFUI_LoginPerfectWorld_AntiAddiction1") { start += 600; }
					if ( p == '\"' )
					{
						if ( qcount < 4 )
							vpos[qcount] = start;
						++qcount;
					}
					else if ( p == '\n' )
					{
						if ( qcount == 4 )
							_variables[string( const_cast< char* >( &file->at( vpos[0] + 1 ) ) , vpos[1] - vpos[0] - 1 )] = string( const_cast< char* >( &file->at( vpos[2] + 1 ) ) , vpos[3] - vpos[2] - 1 );
						qcount = 0;
					}
					else if ( qcount != 1 && qcount != 3 )
					{
						if ( p == '{' )
						{
							vpos[0] = file->FindFirstOf( "\"" , start , 0 ) - 1;
							vpos[1] = file->FindFirstOf( "\"" , vpos[0] , 0 );

							_sublevels.insert( pair<string , CLevel*>( string( const_cast< char* >( &file->at( vpos[1] + 1 ) ) , vpos[0] - vpos[1] ) , new CLevel( start , file ) ) );
						}
						else if ( p == '}' )
							break;
					}
				}
				_end = start;
			}

			CLevel::~CLevel( void )
			{
				for ( auto& p : _sublevels )
				{
					delete p.second;
				}
			}

			CConfig::CConfig( void ) : CFile()
			{
			}

			CConfig::~CConfig( void )
			{
				delete _level;
			}

			bool CConfig::Parse( void )
			{
				auto rstart = FindFirstOf( "{" , 0 , 200 );
				_level = new CLevel( rstart , _instance );

				return !_level->GetSubLevels().empty();
			}
		}
	}

	namespace Skins
	{
		CSkins::CSkins( void )
		{
		}

		CSkins::~CSkins( void )
		{
			delete _items_game;
			delete _csgo_english;
		}

		bool CSkins::DumpTable( File::Valve::mapLevel tab , File::Valve::mapVariable vt , File::Valve::mapLevel ai_wi ) {
			for ( auto& pk : tab )
			{
				auto& pkid = pk.first;

				if ( pkid == "9001" )
					continue;

				auto& pkname = pk.second->GetVariables().at(  "name" );
				auto& pkdesctag = string( pk.second->GetVariables().at( "description_tag" ) );

				auto& res = vt.find( pkdesctag.substr( 1 , pkdesctag.size() + 1 ) );
				if ( res == vt.end() )
				{
					pkdesctag[6] = 'k';
					res = vt.find( pkdesctag.substr( 1 , pkdesctag.size() + 1 ) );
					if ( res == vt.end() )
						continue;
				}

				auto& skname = res->second;
				if ( skname == "-" )
					continue;

				for ( auto& wi : ai_wi )
				{
					auto& vip = wi.second->GetVariables().at( "icon_path" );
					if ( vip[vip.size() - 7] != '_' )
						continue;

					auto res = vip.find( pkname );
					if ( res == string::npos )
						continue;

					_skininfo[vip.substr( 23 , res - 24 )].push_back( { atoi( pkid.c_str() ), pkname, skname } );
				}
			}

			return true;
		}

		bool CSkins::Load( const string& gamePath , const string& gameShortName )
		{
			Release();

			_items_game = new File::Valve::CConfig();
			if ( !_items_game->Load( gamePath + "/scripts/items/items_game.txt" , false ) )
				return false;

			_csgo_english = new File::Valve::CConfig();
			if ( !_csgo_english->Load( gamePath + "/resource/" + gameShortName + "_english.txt" , true ) )
				return false;

			// honestly this entire fix is so fucking ghetto kill me please
			auto fnFind = ( []( multimap<string , File::Valve::CLevel*> map , string test ) -> File::Valve::CLevel* {
				auto& lol = map.equal_range( test );
				for ( auto it = lol.first; it != lol.second; ++it )
				{
					return it->second;
				}
			} );

			// ughgughu
			auto& vt = fnFind( _csgo_english->GetLevel()->GetSubLevels() , "Tokens" )->GetVariables();
			auto mhhh = fnFind( _items_game->GetLevel()->GetSubLevels() , "alternate_icons2" );
			auto mhh = fnFind( mhhh->GetSubLevels() ,  "weapon_icons" );
			auto& ai_wi = mhh->GetSubLevels();

			for ( auto& wat : _items_game->GetLevel()->GetSubLevels() )
			{
				if ( wat.first.find( "paint_kits" ) != string::npos )
				{
					// meh
					DumpTable( wat.second->GetSubLevels() , vt , ai_wi );
				}
			}

			return true;
		}

		void CSkins::Release( void )
		{
			if ( _items_game )
				_items_game->Release();
			if ( _csgo_english )
				_csgo_english->Release();
		}
	}
}