#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
//#include "PQueue.h"
//#include "DynArray.h"

#include "PugiXml\src\pugixml.hpp"

#define COST_MAP_SIZE 25

struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;
	int columns;
	int tilecount;

	SDL_Texture* texture;

	SDL_Rect GetTileRect(int gid) const;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct Properties
{
	struct Property
	{
		SString name;
		bool value;
	};

	~Properties()
	{
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.Clear();
	}

	Property* GetProperty(const char* name);

	List<Property*> list;
};

struct MapLayer
{
	SString	name;
	int id; 
	int width;
	int height;
	uint* data;

	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	List<TileSet*> tilesets;
	MapTypes type;

	List<MapLayer*> maplayers;
};

class Map : public Module
{
public:

	//List<PhysBody*> colliders; //CUIDADO

    Map(App* application, bool start_enabled = false);

    virtual ~Map();

    bool Awake(pugi::xml_node& conf);

	bool WalkMap(int& width, int& height, uchar** buffer) const; //SUSSY12 Create Walkability Map

    void Draw();

    bool CleanUp();

    bool Load();

	iPoint MapToWorld(int x, int y) const;

	iPoint Map::WorldToMap(int x, int y);

	// BFS/Dijkstra methods not required any more: Using PathFinding class
/*
// L09: BFS Pathfinding methods
void ResetPath();
void DrawPath();
bool IsWalkable(int x, int y) const;

// L10: Methods for BFS + Pathfinding and cost function for Dijkstra
int MovementCost(int x, int y) const;
void ComputePath(int x, int y);

// Propagation methods
void PropagateBFS(); //L09
void PropagateDijkstra(); //L10
void PropagateAStar(int heuristic); //L11
*/

private:

	bool LoadMap(pugi::xml_node mapFile);

	bool LoadTileSet(pugi::xml_node mapFile);

	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);

	TileSet* GetTilesetFromTileId(int gid) const;

	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	void LoadCollisionsFromTileId();



public: 

	MapData mapData;
	List<PhysBody*> enemies;
	List<PhysBody*> mapColliders;

private:

    SString mapFileName;
	SString mapFolder;
    bool mapLoaded;

	// BFS/Dijkstra methods not required any more: Using PathFinding class
/*
// L09: BFS Pathfinding variables
PQueue<iPoint> frontier;
List<iPoint> visited;

// L09 DONE 4: Define destionation point
iPoint destination;

// L10: Additional variables
List<iPoint> breadcrumbs;
uint costSoFar[COST_MAP_SIZE][COST_MAP_SIZE];
DynArray<iPoint> path;

SDL_Texture* tileX = nullptr;
*/
};

#endif // __MAP_H__