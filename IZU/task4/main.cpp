#include <limits>
#include <iostream>
#include "cluster.hpp"


void inicializedBufferVector(std::vector<std::shared_ptr<Point>> & buffer){
    buffer.clear();
    buffer.push_back(std::make_shared<Point>( 1, 7,-9, 1) );
    buffer.push_back(std::make_shared<Point>(-1,-3, 3, 2) );
    buffer.push_back(std::make_shared<Point>(-8, 9, 8, 3) ); 
    buffer.push_back(std::make_shared<Point>( 4, 2,-5, 4) ); 
    buffer.push_back(std::make_shared<Point>(-8,-8, 8, 5) ); 
    buffer.push_back(std::make_shared<Point>( 6, 5,-4, 6) ); 
    buffer.push_back(std::make_shared<Point>( 7, 2, 7, 7) ); 
    buffer.push_back(std::make_shared<Point>( 8,-4, 0, 8) ); 
    buffer.push_back(std::make_shared<Point>(-1, 0, 7, 9) ); 
    buffer.push_back(std::make_shared<Point>(-2,-1, 0, 10) );
}

void initClusters(std::vector<std::shared_ptr<Cluster>> & clusters  ){
    clusters.clear();
    clusters.push_back( std::make_shared<Cluster>( -8, 9, 8));
    clusters.push_back( std::make_shared<Cluster>(  7, 2, 7));
    clusters.push_back( std::make_shared<Cluster>( -8, -8, 8));
}



int main(){
    std::vector<std::shared_ptr<Point>> buffer;
    inicializedBufferVector(buffer);

    std::vector<std::shared_ptr<Cluster>> clusters;
    initClusters(clusters);

    bool hasChanged = true;
    for(int step = 1;hasChanged; ++step){
        // na zacatek predpokladat, ze se nic nezmeni
        hasChanged = false;
        // pro kazdy bod
        for( auto currentPoint : buffer ){
            // najit nejblizsi stred
            std::shared_ptr<Cluster> closestCluster;
            double closestDistance = __DBL_MAX__;
            for( auto currentCluster : clusters ){
                double currentDistance =  currentCluster->calculateDistanceFromCenter(currentPoint);
                if( currentDistance < closestDistance ){
                    closestCluster = currentCluster;
                    closestDistance = currentDistance;
                }
            }
            // zjistit jestli bude menit cluster, to i kdyz se pouze vklada
            if( closestCluster->checkIfClusterContainsPoint(currentPoint) == false ){
                // pokud ano
                    // najit cluster kde je ulozeny a erase
                    for( auto clusterForRemove : clusters ){
                        if( clusterForRemove->checkIfClusterContainsPoint(currentPoint) ){
                            clusterForRemove->removePointFromCluster(currentPoint);
                            break;
                        }
                    }
                    // vlozit do noveho clusteru
                    closestCluster->addPointIntoCluster(currentPoint);
                    // nastavit has Changed na TRUE
                    hasChanged = true;
            }
        }
        // vytisknout stav po
        std::cout << "Krok" << step << ":----------------------" << std::endl;
        int n = 1;
        for( auto current : clusters ){
            std::cout << "Cluster" << n << ":" << std::endl;
            current->printCluster();
            ++n;
        }
        for( auto cluster : clusters ) cluster->calculateCenter();
    }



    return 0;
}