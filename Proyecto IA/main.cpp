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
vector<vector<vector<bool>>> domain;

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
    for (vector<bool> vect : CurrentSolution)
    {
        for (bool item : vect)
        {
            cout << item << " ";
        }
        cout << endl;
    }

}

void showDomain(){
    for (vector<vector<bool>> vect : domain)
    {
        for (vector<bool> item : vect)
        {
            cout << "(";
            for(bool item2:item){
                cout << item2;
                
            }
            cout << ") ";
        }
        cout << endl;
    }
}

void initializeDomain(){//Inicia la matriz de dominios
    for(int i=0;i<entities.size();i++){
        vector<vector<bool>> aux;
        for(int j=0;j<rooms.size();j++){
            vector<bool> aux2;
            aux2.push_back(false);
            aux2.push_back(true);
            aux.push_back(aux2);
        }
        domain.push_back(aux);
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
int getRoom(int ent){//Returns the room of the entity. Returns -1 if the entity is not allocated.
    for(int i=0;i<FutureSolution[0].size();i++){
        if(FutureSolution[ent][i]==1){
            return i;
        }
    }
    return -1;
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
bool checkFutureSolution(){
    int count;
    for (vector<bool> ent : FutureSolution){//Check entity is only on one room
        count=0;
        for (bool rom : ent){
            if(rom){
                count++;
            }
            if(count>1){
                return false;
            }
        }
    }
    
    for(constraint cons: constraints){
        if(cons.type==-1){//Unused Constraint
            continue;
        }
        else if(cons.type==0 && cons.hardness==true){//Allocation constraint
            if(FutureSolution[cons.subject][cons.target]==0){
                return false;
            }
        }
        else if(cons.type==1 && cons.hardness==true){//Non allocation constraint
            if(FutureSolution[cons.subject][cons.target]==1){
                return false;
            }
        }
        else if(cons.type==2 && cons.hardness==true){//one of constraint (No usada en el dataset)
            continue;
        }
        else if(cons.type==3 && cons.hardness==true){//capacity constraint
            int spaceUsed=0;
            for(int i=0;i<FutureSolution.size();i++){
                if(FutureSolution[i][cons.subject]==true){
                    spaceUsed+=entities[i].space;
                }
            }
            if(spaceUsed>rooms[cons.subject].space){
                return false;
            }
        }
        else if(cons.type==4 && cons.hardness==true){//sameroom constraint
            if(getRoom(cons.subject)!=getRoom(cons.target)){
                return false;
            }
        }
        else if(cons.type==5 && cons.hardness==true){//not same room constraint
            if(getRoom(cons.subject)==getRoom(cons.target)){
                return false;
            }
        }
        else if(cons.type==6 && cons.hardness==true){//not sharing constraint
            for(int i=0;i<entities.size();i++){
                if(i==cons.subject){
                    continue;
                }
                if(getRoom(cons.subject)==getRoom(i)){
                    return false;
                }
            }
        }
        else if(cons.type==7 && cons.hardness==true){//adjacency constraint
            bool isAdjacent=false;
            for(int item: rooms[getRoom(cons.subject)].adjacentRooms){
                if(item==rooms[getRoom(cons.subject)].id){
                    isAdjacent=true;
                }
            }
            if(!isAdjacent){
                return false;
            }
        }
        else if(cons.type==8 && cons.hardness==true){//neaby constraint
            if(rooms[getRoom(cons.subject)].floor!=rooms[getRoom(cons.target)].floor){
                return false;
            }
        }
        else if(cons.type==9 && cons.hardness==true){//away from constraint
            if(rooms[getRoom(cons.subject)].floor==rooms[getRoom(cons.target)].floor){
                return false;
            }
        }
        
    }
    return true;
}

void forwardChecking(){
    for(int i=0;i<entities.size();i++){
        for(int j=0;j<rooms.size();j++){
            vector<bool>::iterator it;
            for(it=domain[i][j].begin();it!=domain[i][j].end();++it){
                FutureSolution[i][j]=*it;
                if(!checkFutureSolution()){
                    domain[i][j].erase(it);
                }
            }
            if(domain[i][j].size()==0){
                cout << "backtrack";
                return;
            }
            CurrentSolution[i][j]=FutureSolution[i][j]=domain[i][j][0];
        }
    }
}
int main(int argc, const char * argv[]) {
    readInstance("/Users/carlos/Downloads/OSAP-Instancias/nott_data/nott1d.txt");
    initializeSolutions();
    FutureSolution=CurrentSolution;
    initializeDomain();
    forwardChecking();
    showDomain();
    //showSolution();
    return 0;
}
