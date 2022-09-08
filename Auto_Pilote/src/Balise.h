#ifndef _BALISE_H
#define _BALISE_H

////////////////////////////////////////////////////////////////////////////////
// HEAD
#include <iostream>
#include <vector>


class Tag{
	public:
	Tag(int x=0, int y=0):posX(x), posY(y) {};
	int posX;
	int posY;
};

class FindTag{
	public:
	FindTag(int x, int y, int dis=0, int prev=0):posX(x), posY(y), distance(dis), previous(prev) {};
	FindTag(Tag &balise, int dis=0, int prev=0):posX(balise.posX), posY(balise.posY), distance(dis), previous(prev) {};
	int posX;
	int posY;
	int distance;
	int previous;
};

class PositionTag{
	public:
	PositionTag(int deg, string name) : angle(deg), posX(0), posY(0), name(name) {};
	PositionTag(int x, int y, int deg, string name, double range=0) : posX(x), posY(y), angle(deg), name(name), range(range) {};
	int posX;
	int posY;
	int angle;
	string name;
	double range;
};

/*####****************** FONCTEUR TO FIND CONTAIN IN A VEVCTOR ******************####*/
class FindVecteur{
	public:
	FindVecteur(int x, int y) : posX(x), posY(y){};
	bool operator ()(const Tag &balise) const{
		return (balise.posX == this->posX && balise.posY == this->posY);
	}
	private:
	int posX;
	int posY;
};

/*####****************** FONCTEUR TO FIND CONTAIN IN A VEVCTOR ******************####*/
class FindVecteurName{
	public:
	FindVecteurName(string name) : nameBalise(name){};
	bool operator ()(const PositionTag &balise) const{
		return (balise.name == this->nameBalise);
	}
	private:
	string nameBalise;
};

/*####****************** FONCTEUR TO FIND CONTAIN IN A VEVCTOR ******************####*/
class FindVecteurNextTag{
	public:
	FindVecteurNextTag(int x, int y) : posX(x), posY(y){};;
	bool operator ()(const FindTag &balise) const{
		return ((balise.posX == this->posX && balise.posY != this->posY) || (balise.posX != this->posX && balise.posY == this->posY));
	}
	private:
	int posX;
	int posY;
};
#endif