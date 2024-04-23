#include "point.hpp"
#include <memory>
#include <vector>
#include <cmath>

class Cluster
{
private:
    std::vector<std::shared_ptr<Point>> cluster;
    std::shared_ptr<Point> center;
public:
    Cluster(double x,double y, double z);
    ~Cluster();
    void calculateCenter();
    double calculateDistanceFromCenter(std::shared_ptr<Point> point);
    void addPointIntoCluster( std::shared_ptr<Point> point );
    void removePointFromCluster(std::shared_ptr<Point> point);
    bool checkIfClusterContainsPoint(std::shared_ptr<Point> point);
    void printCluster();
};

Cluster::Cluster(double x,double y, double z)
{
    this->center = std::make_shared<Point>(x,y,z);
    this->cluster.clear();
}

Cluster::~Cluster()
{
}

void Cluster::calculateCenter(){
    double centerX = 0,centerY = 0,centerZ = 0;
    for( auto point : cluster){
        centerX += point->x;
        centerY += point->y;
        centerZ += point->z;
    }
    this->center->x = centerX / cluster.size();
    this->center->y = centerY / cluster.size();
    this->center->z = centerZ / cluster.size();
}

double Cluster::calculateDistanceFromCenter(std::shared_ptr<Point> point){
    return std::sqrt( std::pow((center->x - point->x),2) 
                    + std::pow((center->y - point->y),2) 
                    + std::pow((center->z - point->z),2) );
}

void Cluster::addPointIntoCluster( std::shared_ptr<Point> point ){
    this->cluster.push_back(point);
}

void Cluster::removePointFromCluster(std::shared_ptr<Point> point){
    for(unsigned int i = 0; i < this->cluster.size(); ++i ){
        if( this->cluster[i]->id == point->id ){
            this->cluster.erase(this->cluster.begin() +i);
            return;
        }
    }
}

bool Cluster::checkIfClusterContainsPoint(std::shared_ptr<Point> point){
    for( auto tmp : this->cluster ){
        if( tmp->id == point->id ) return true;
    }
    return false;
}

void Cluster::printCluster(){
    std::cout << "Stred: ";
    this->center->printPoint();
    std::cout << std::endl;
    std::cout << "Body: ";
    for( auto point : this->cluster ){
        point->printPoint();
        std::cout << " ";
    }
    std::cout << std::endl;
}