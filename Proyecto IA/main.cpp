//
//  main.cpp
//  Proyecto IA
//
//  Created by Carlos Molina Yugovic on 07-06-17.
//  Copyright © 2017 Carlos Molina Yugovic. All rights reserved.
//


#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

struct entity{
    int id;
    int groupId;
    float space;
};

struct room{
    int id;
    int floor;
    float space;
    vector<int> adjacentRooms;
};

struct constraint{
    int id;
    int type;
    bool hardness;
    int subject;
    int target;
};

vector<vector<bool>> CurrentSolution;
vector<vector<bool>> FutureSolution;
vector<entity> entities;
vector<room> rooms;
vector<constraint> constraints;
vector<vector<vector<float>>> domain;

entity readEntity(string line){//Lee una entidad a partir de una linea del .txt
    entity ent;
    int step=0;
    string aux="";
    bool reading=false;
    for(char& c : line){
        if(step==0 && c==' '){//Read id
            if(reading==true){
                step=1;
                reading=false;
                ent.id=stoi(aux);
                aux="";
            }
            continue;
        }
        else if(step==0 && c!=' '){
            aux+=c;
            reading=true;
        }
        
        else if(step==1 && c==' '){//Read group id
            if(reading==true){
                step=2;
                reading=false;
                ent.groupId=stoi(aux);
                aux="";
            }
            continue;
        }
        else if(step==1 && c!=' '){
            aux+=c;
            reading=true;
        }
        else if(step==2 && c==' '){//Read space
            continue;
        }
        else if(step==2 && c!=' '){
            aux+=c;
        }
        
            
    }
    ent.space=stof(aux);
    return ent;
}

room readRoom(string line){//Lee una habitacion a partir de una linea del .txt
    room rom;
    int step=0;
    string aux="";
    bool reading=false;
    for(char& c : line){
        if(step==0 && c==' '){//Read ID
            if(reading==true){
                step=1;
                reading=false;
                rom.id=stoi(aux);
                aux="";
            }
            continue;
        }
        else if(step==0 && c!=' '){
            aux+=c;
            reading=true;
        }
        
        else if(step==1 && c==' '){//Read floor
            if(reading==true){
                step=2;
                reading=false;
                rom.floor=stoi(aux);
                aux="";
            }
            continue;
        }
        else if(step==1 && c!=' '){
            aux+=c;
            reading=true;
        }
        
        else if(step==2 && c==' '){//Read space
            if(reading==true){
                step=3;
                reading=false;
                rom.space=stof(aux);
                aux="";
            }
            continue;
        }
        else if(step==2 && c!=' '){
            aux+=c;
            reading=true;
        }
        else if(step==3 && c==' '){//Read list
            if(reading==true){
                step=4;
                reading=false;
                aux="";
            }
            continue;
        }
        else if(step==3 && c!=' '){
            aux+=c;
            reading=true;
        }
        else if(step==4 && c==' '){//Read list
            if(reading==true){
                reading=false;
                rom.adjacentRooms.push_back(stoi(aux));
                aux="";
            }
            continue;
        }
        else if(step==4 && c!=' '){
            aux+=c;
            reading=true;
        }
        
        
        
    }
    
    if(step==4)
        rom.adjacentRooms.push_back(stoi(aux));
    return rom;
}

constraint readConstraint(string line){//Lee una restriccion a partir de una linea del .txt
    constraint cons;
    int step=0;
    string aux="";
    bool reading=false;
    for(char& c : line){
        if(step==0 && c==' '){//Read id
            if(reading==true){
                step=1;
                reading=false;
                cons.id=stoi(aux);
                aux="";
            }
            continue;
        }
        else if(step==0 && c!=' '){
            aux+=c;
            reading=true;
        }
        
        else if(step==1 && c==' '){//Read type
            if(reading==true){
                step=2;
                reading=false;
                cons.type=stoi(aux);
                aux="";}
            continue;
        }
        else if(step==1 && c!=' '){
            aux+=c;
            reading=true;
        }
        
        else if(step==2 && c==' '){//Read hardness
            if(reading==true){
                step=3;
                reading=false;
                cons.hardness=stoi(aux);
                aux="";
            }
            continue;
        }
        else if(step==2 && c!=' '){
            aux+=c;
            reading=true;
        }
        
        else if(step==3 && c==' '){//Read subject
            if(reading==true){
                step=4;
                reading=false;
                cons.subject=stoi(aux);
                aux="";
            }
            continue;
        }
        else if(step==3 && c!=' '){
            aux+=c;
            reading=true;
        }
        
        else if(step==4 && c==' '){//Read target
            continue;
        }
        else if(step==4 && c!=' '){
            aux+=c;
        }
        
        
    }
    cons.target=stoi(aux);
    return cons;
}

void showEntities(){//Muestra las entidades cargadas en memoria
    vector<entity>::iterator it;
    for (it=entities.begin(); it != entities.end(); ++it){
        cout << (*it).id << " " << (*it).groupId << " " << (*it).space << "\n";
    }
    cout << "\n";
    
}

void showRooms(){//Muestra las habitaciones cargadas en memoria
    vector<room>::iterator it2;
    for (it2=rooms.begin(); it2 != rooms.end(); ++it2){
        cout << (*it2).id << " " << (*it2).floor << " " << (*it2).space << " ";
        for(vector<int>::iterator it4=(*it2).adjacentRooms.begin();it4!=(*it2).adjacentRooms.end();++it4){
            cout << *it4 << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void showConstraints(){//Muestra las restricciones cargadas en memoria
    vector<constraint>::iterator it3;
    for (it3=constraints.begin(); it3 != constraints.end(); ++it3){
        cout << (*it3).id << " " << (*it3).type << " " << (*it3).hardness << " " << (*it3).subject << " " << (*it3).target << "\n";
    }
}

void showSolution(){//Muestra la solucion encontrada
    /*vector<vector<bool>>::iterator it3;
    for (it3=CurrentSolution.begin(); it3 != CurrentSolution.end(); ++it3){
        for (vector<bool>::iterator it=(*it3).begin(); it != (*it3).end(); ++it){
            cout << *it << " ";
        }
                cout << "\n";
    }*/
    for (vector<bool> vect : CurrentSolution)
    {
        for (bool item : vect)
        {
            cout << item << " ";
        }
        cout << endl;
    }

}

void initializeSolutions(){//Inicializa la matriz de soluciones en puros 0
    
    for(int i=0;i<entities.size();i++){
        vector<bool> aux;
        for(int j=0;j<rooms.size();j++){
            aux.push_back(false);
        }
        CurrentSolution.push_back(aux);
        
    }
}

void readInstance(string path){//Lee la instancia de datos a partir de la direccion del archivo
    string line;
    ifstream file;
    file.open(path);
    while ( getline (file,line) ){
        if(line[0]=='E'){
            break;
        }
        
    }
    while ( getline (file,line) ){//READ ENTITIES
        if(line.size()<2){
            continue;
        }
        if(line[0]=='R'){
            break;
        }
        entities.push_back(readEntity(line));

    }
    
    while ( getline (file,line) ){//READ ROOMS
            if(line.size()<2){
                continue;
            }

            if(line[0]=='C'){
                break;
            }
            rooms.push_back(readRoom(line));
    }
    
    while ( getline (file,line) ){//READ CONSTRAINTS
        if(line.size()<2){
            continue;
        }
        constraints.push_back(readConstraint(line));
    }
    file.close();
    return;
}
void checkFutureSolution(){
    for(int i=0;i<FutureSolution.size();i++){
        for(int j=0;j<FutureSolution[0].size();j++){
            
        }
 
    }
    for(constraint cons: constraints){
        
    }
}
void forwardChecking(){
    for (vector<bool> vect : FutureSolution){
        for (bool item : vect){
            
        }
    }

}

int main(int argc, const char * argv[]) {
    readInstance("/Users/carlos/Downloads/OSAP-Instancias/nott_data/nott1d.txt");
    initializeSolutions();
    showSolution();
    return 0;
}
