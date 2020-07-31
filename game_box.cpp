#include "game_box.h"

tsObjectID id1, id2, id3, id4, id5, id6;

void GameBox::CalculateStep()
{


	if (!gameboxActors[0])
	{
		id1 = objects.addObject(Rectangle({ 10,10 }, 4, 5, { 15,0,10 }));
		id2 = objects.addObject(Rectangle({ 8,8 }, 3, 3, { 5,20,0 }));
		//id2 = objects.addObject(Circle({ 4,4 }, 3,{ 10,5,0}));
		//id4 = objects.addObject(Text({ 0,0 },"P1",{ 15,5,1 }));
		//id5 = objects.addObject(Text({ 9,0 }, "P2", { 5,15,1 }));

		id6 = objects.addObject(Text({ 0,0 }, "COLL", { 0,0,0 }));

		gameboxActors[0] = 1;
	    gameboxActors[1] = 1;
		gameboxActors[2] = 1;

	}

	if (objects.checkCollision(id1, id2))
	{
		objects.setColor(id6, { 30,0,0 });



	}
	else
	{
		objects.setColor(id6, { 0,30,0 });
	}

	objects.setPosition(id1, { objects.getPosition(id1).x + gameboxActors[1]/5,objects.getPosition(id1).y});
	objects.setPosition(id2, { objects.getPosition(id2).x,objects.getPosition(id2).y + gameboxActors[2] / 3.25 });


	if (objects.getPosition(id1).x > 18)
	{
		gameboxActors[1] = -1;
	}
	else if(objects.getPosition(id1).x < -5)
	{
		gameboxActors[1] = 1;
	}

	if (objects.getPosition(id2).y > 18)
	{
		gameboxActors[2] = -1;
	}
	else if (objects.getPosition(id2).y < -5)
	{
		gameboxActors[2] = 1;
	}

	//objects.deactivate(id3);

	//objects.deactivate(id1);
	//objects.deactivate(id2);
	//objects.deactivate(id3);

	//id = objects.addObject(Text("000"));
	//objects.setPosition(id, {1,1});
	//objects.setColor(id, {50,50,50});
	//objects.removeObject(id);
	//objects.resetObjectCollection();

	//switch (state)
	//{
	//case mainmenu:


	//	break;


	//default:
	//	break;
	//}

}