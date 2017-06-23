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

struct Entity{
    int id;
    int groupId;
    float space;
};

struct Room{
    int id;
    int floor;
    float space;
    vector<int> adjacentRooms;
};

struct Constraint{
    int id;
    int type;
    bool hardness;
    int subject;
    int target;
};

//Penalties for constraints
int p0=20;  //Allocation
int p1=10;  //Non Allocation
int p2=10;  //One Of
int p3=10;  //capacity
int p4=10;  //same room
int p5=10;  //not same room
int p6=50;  //not sharing
int p7=10;  //adjacency
int p8=10;  //nearby
int p9=10;  //away

int iterations=10000;//Solutions found until the algorithm stops
vector<vector<int>> graphConnections;
vector<int> order;
vector<int> solution;
vector<int> bestSolution;
vector<vector<int>> domain;
vector<Entity> entities;
vector<Room> rooms;
vector<Constraint> constraints;
vector<vector<Constraint>> constraintsPerEntity;
void initSolution(){
    for(int i=0;i<entities.size();i++){
        solution.push_back(-1);
        bestSolution.push_back(-1);
    }
}
void initDomain(){
    for(int i=0;i<entities.size();i++){
        vector<int> aux;
        for(int j=0;j<rooms.size();j++){
            aux.push_back(j);
        }
        domain.push_back(aux);
    }
}
void showDomain(){
    for(vector<int> vect:domain){
        for (int i:vect) {
            cout << i << " ";
        }
        cout << endl;
    }
    cout << endl;
}
void showSolution(vector<int> sol){
    for (int i:sol) {
        cout << i << " ";
    }
    cout << endl;
}
void initGraphConnections(){
    for(Constraint cons: constraints){
        if(cons.type==4 && cons.hardness==true){//sameroom constraint
            graphConnections[cons.target].push_back(cons.subject);
            graphConnections[cons.subject].push_back(cons.target);
        }
        else if(cons.type==5 && cons.hardness==true){//not same room constraint
            graphConnections[cons.target].push_back(cons.subject);
            graphConnections[cons.subject].push_back(cons.target);
        }
        else if(cons.type==6 && cons.hardness==true){//not sharing constraint
            for(int i=0;i<entities.size();i++){
                if(i!=cons.subject){
                    graphConnections[cons.subject].push_back(i);
                    graphConnections[i].push_back(cons.subject);
                }
            }
        }
    }
        
}

int jumpbackGBJ(int ent){
    
    
    return ent;
}



Entity readEntity(string line){//Lee una entidad a partir de una linea del .txt
    Entity ent;
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

Room readRoom(string line){//Lee una habitacion a partir de una linea del .txt
    Room rom;
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

Constraint readConstraint(string line){//Lee una restriccion a partir de una linea del .txt
    Constraint cons;
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
    vector<Entity>::iterator it;
    for (it=entities.begin(); it != entities.end(); ++it){
        cout << (*it).id << " " << (*it).groupId << " " << (*it).space << "\n";
    }
    cout << "\n";
    
}

void showRooms(){//Muestra las habitaciones cargadas en memoria
    vector<Room>::iterator it2;
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
    vector<Constraint>::iterator it3;
    for (it3=constraints.begin(); it3 != constraints.end(); ++it3){
        cout << (*it3).id << " " << (*it3).type << " " << (*it3).hardness << " " << (*it3).subject << " " << (*it3).target << "\n";
    }
}
 
void readInstance(string path){//Lee la instancia de datos a partir de la direccion del archivo
    string line;
    ifstream file;
    file.open(path);
    while ( getline (file,line) ){
        if(line.size()<2){
            continue;
        }
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

bool checkDomain(int ent, int room){//checks if the domain value is feasible
    for(Constraint cons: constraints){
        if(cons.type==-1){//Unused Constraint
            continue;
        }
        else if(cons.type==0 && cons.hardness==true && cons.subject==ent){//Allocation constraint
            if(cons.target!=room){
                return false;
            }
        }
        else if(cons.type==1 && cons.hardness==true && cons.subject==ent){//Non allocation constraint
            if(cons.target==room){
                return false;
            }
        }
        else if(cons.type==2 && cons.hardness==true){//one of constraint (Not used on the dataset)
            continue;
        }
        else if(cons.type==3 && cons.hardness==true && cons.subject==room){//capacity constraint
            int spaceUsed=0;
            for(int i=0;i<solution.size();i++){
                if(solution[i]==room){
                    spaceUsed+=entities[i].space;
                }
            }
            if(spaceUsed>rooms[room].space){
                return false;
            }
        }
        else if(cons.type==4 && cons.hardness==true && (cons.subject==ent || cons.target==ent)){//sameroom constraint
            if(cons.subject==ent){
                if(solution[cons.target]==-1){
                    continue;}
                if(solution[cons.target]!=room){
                    return false;
                }
            }
            else if(cons.target==ent){
                if(solution[cons.subject]==-1){
                    continue;}
                if(solution[cons.subject]!=room){
                    return false;
                }
            }
        }
        else if(cons.type==5 && cons.hardness==true && (cons.subject==ent || cons.target==ent)){//not same room constraint
            if(cons.subject==ent){
                if(solution[cons.target]==-1){
                    continue;}
                if(solution[cons.target]==room){
                    return false;
                }
            }
            else if(cons.target==ent){
                if(solution[cons.subject]==-1){
                    continue;}
                if(solution[cons.subject]==room){
                    return false;
                }
            }
        }
        else if(cons.type==6 && cons.hardness==true){//not sharing constraint
            if(ent==cons.subject){
                for(int i=0;i<entities.size();i++){
                    if(solution[i]==room){
                        return false;
                    }
                }
            }
            else if(solution[cons.subject]==room){
                return false;
            }
        }
        else if(cons.type==7 && cons.hardness==true && (cons.target==ent || cons.subject==ent)){//adjacency constraint
            bool isAdjacent=false;
            if(cons.subject==ent){
                if(solution[cons.target]==-1){
                    continue;}
                for(int item: rooms[room].adjacentRooms){
                    if(item==solution[cons.target]){
                        isAdjacent=true;
                    }
                }
            }
            else if(cons.target==ent){
                if(solution[cons.subject]==-1){
                    continue;}
                for(int item: rooms[room].adjacentRooms){
                    if(item==solution[cons.subject]){
                        isAdjacent=true;
                    }
                }
            }

            
            if(!isAdjacent){
                return false;
            }
        }
        else if(cons.type==8 && cons.hardness==true && (cons.target==ent || cons.subject==ent)){//neaby constraint
            if(cons.subject==ent){
                if(solution[cons.target]==-1){
                    continue;}
                if(rooms[room].floor!=rooms[solution[cons.target]].floor){
                    return false;
                }
            }
            else if(cons.target==ent){
                if(solution[cons.subject]==-1){
                    continue;}
                if(rooms[room].floor!=rooms[solution[cons.subject]].floor){
                    return false;
                }
              
            }
        }
        else if(cons.type==9 && cons.hardness==true && (cons.target==ent || cons.subject==ent)){//away from constraint
            if(cons.subject==ent){
                if(solution[cons.target]==-1){
                    continue;}
                if(rooms[room].floor==rooms[solution[cons.target]].floor){
                    return false;
                }
            }
            else if(cons.target==ent){
                if(solution[cons.subject]==-1){
                    continue;}
                if(rooms[room].floor==rooms[solution[cons.subject]].floor){
                    return false;
                }
                
            }
        }
        
    }
    return true;
}

void initConstrainsPerEntity(){//initialize constraint per entity
    for(Constraint cons: constraints){
        if(cons.type==0){//Allocation constraint
            constraintsPerEntity[cons.subject].push_back(cons);
        }
        else if(cons.type==1){//Non allocation constraint
            constraintsPerEntity[cons.subject].push_back(cons);
        }
        else if(cons.type==3){//capacity constraint
            for(int i=0; i<constraintsPerEntity.size();i++){
                constraintsPerEntity[i].push_back(cons);
            }
        }
        else if(cons.type==4){//sameroom constraint
            constraintsPerEntity[cons.subject].push_back(cons);
            constraintsPerEntity[cons.target].push_back(cons);
        }
        else if(cons.type==5){//not same room constraint
            constraintsPerEntity[cons.subject].push_back(cons);
            constraintsPerEntity[cons.target].push_back(cons);
        }
        else if(cons.type==6){//not sharing constraint
            for(int i=0; i<constraintsPerEntity.size();i++){
                constraintsPerEntity[i].push_back(cons);
            }
        }
        else if(cons.type==7){//adjacency constraint
            constraintsPerEntity[cons.subject].push_back(cons);
            constraintsPerEntity[cons.target].push_back(cons);
        }
        else if(cons.type==8){//neaby constraint
            constraintsPerEntity[cons.subject].push_back(cons);
            constraintsPerEntity[cons.target].push_back(cons);
        }
        else if(cons.type==9){//away from constraint
            constraintsPerEntity[cons.subject].push_back(cons);
            constraintsPerEntity[cons.target].push_back(cons);
        }
        
    }
}

int penalties(){//returns the penalties of soft constraints
    int penalties=0;
    for(Constraint cons: constraints){
        if(cons.type==-1){//Unused Constraint
            continue;
        }
        else if(cons.type==0 && cons.hardness==false){//Allocation constraint
            if(cons.target!=solution[cons.subject]){
                penalties+=p0;
            }
        }
        else if(cons.type==1 && cons.hardness==false){//Non allocation constraint
            if(cons.target==solution[cons.subject]){
                penalties+=p1;
            }
        }
        else if(cons.type==2 && cons.hardness==false){//one of constraint (Not used on the dataset)
            continue;
        }
        else if(cons.type==3 && cons.hardness==false){//capacity constraint
            float spaceUsed=0;
            for(int i=0;i<solution.size();i++){
                if(solution[i]==cons.subject){
                    spaceUsed+=entities[i].space;
                }
            }
            if(spaceUsed>rooms[cons.subject].space){
                penalties+=p3;
            }
        }
        else if(cons.type==4 && cons.hardness==false){//sameroom constraint
            if(solution[cons.target]!=solution[cons.subject]){
                penalties+=p4;
            }
        }
        else if(cons.type==5 && cons.hardness==false){//not same room constraint
            if(solution[cons.target]==solution[cons.subject]){
                penalties+=p5;
            }
        }
        else if(cons.type==6 && cons.hardness==false){//not sharing constraint
            for(int i=0;i<entities.size();i++){
                if(solution[i]==solution[cons.subject] && i!=cons.subject){
                    penalties+=p6;
                }
            }
        }
        else if(cons.type==7 && cons.hardness==false){//adjacency constraint
            bool isAdjacent=false;
            for(int item: rooms[solution[cons.subject]].adjacentRooms){
                if(item==solution[cons.target]){
                    isAdjacent=true;
                }
            }
            if(!isAdjacent){
                penalties+=p7;
            }
        }
        else if(cons.type==8 && cons.hardness==false){//neaby constraint
            if(rooms[solution[cons.subject]].floor!=rooms[solution[cons.target]].floor){
                penalties+=p8;
            }
           
        }
        else if(cons.type==9 && cons.hardness==false){//away from constraint
            if(rooms[solution[cons.subject]].floor==rooms[solution[cons.target]].floor){
                penalties+=p9;
            }
        }
        
    }
    return penalties;
}

int objectiveFunction(vector<int> sol){
    float overusedSpace=0;
    float underusedSpace=0;
    vector<float> usedSpace;
    for (int i=0;i<rooms.size();i++){
        usedSpace.push_back(0);
    }
    for(int i=0;i<sol.size();i++){
        if(sol[i]==-1){
            return 1000000000;
        }
        usedSpace[sol[i]]+=entities[i].space;
    }
    for(int i=0;i<rooms.size();i++){
        if(rooms[i].space>usedSpace[i]){
            underusedSpace+=(rooms[i].space-usedSpace[i]);
        }
        else if(rooms[i].space<usedSpace[i]){
            overusedSpace+=(usedSpace[i]-rooms[i].space);
        }
    }
    
    return penalties()+underusedSpace+2*overusedSpace;
}

void generateOutput(vector<int> sol){//Genera el archivo de output
    int nRB=0;
    string softCons="";
    ofstream myfile;
    myfile.open ("/Users/carlos/Downloads/OSAP-Instancias/nott_data/output.txt",ios::app);
    for(Constraint cons: constraints){
        if(cons.type==-1){//Unused Constraint
            continue;
        }
        else if(cons.type==0 && cons.hardness==false){//Allocation constraint
            if(cons.target!=sol[cons.subject]){
                nRB+=1;
                softCons+="\t";
                softCons+=to_string(cons.id);
            }
        }
        else if(cons.type==1 && cons.hardness==false){//Non allocation constraint
            if(cons.target==sol[cons.subject]){
                nRB+=1;
                softCons+="\t";
                softCons+=to_string(cons.id);
            }
        }
        else if(cons.type==2 && cons.hardness==false){//one of constraint (Not used on the dataset)
            continue;
        }
        else if(cons.type==3 && cons.hardness==false){//capacity constraint
            float spaceUsed=0;
            for(int i=0;i<sol.size();i++){
                if(sol[i]==cons.subject){
                    spaceUsed+=entities[i].space;
                }
            }
            if(spaceUsed>rooms[cons.subject].space){
                nRB+=1;
                softCons+="\t";
                softCons+=to_string(cons.id);
            }
        }
        else if(cons.type==4 && cons.hardness==false){//sameroom constraint
            if(sol[cons.target]!=sol[cons.subject]){
                nRB+=1;
                softCons+="\t";
                softCons+=to_string(cons.id);
            }
        }
        else if(cons.type==5 && cons.hardness==false){//not same room constraint
            if(sol[cons.target]==sol[cons.subject]){
                nRB+=1;
                softCons+="\t";
                softCons+=to_string(cons.id);
            }
        }
        else if(cons.type==6 && cons.hardness==false){//not sharing constraint
            for(int i=0;i<entities.size();i++){
                if(sol[i]==sol[cons.subject] && i!=cons.subject){
                    nRB+=1;
                    softCons+="\t";
                    softCons+=to_string(cons.id);
                }
            }
        }
        else if(cons.type==7 && cons.hardness==false){//adjacency constraint
            bool isAdjacent=false;
            for(int item: rooms[sol[cons.subject]].adjacentRooms){
                if(item==sol[cons.target]){
                    isAdjacent=true;
                }
            }
            if(!isAdjacent){
                nRB+=1;
                softCons+="\t";
                softCons+=to_string(cons.id);
            }
        }
        else if(cons.type==8 && cons.hardness==false){//neaby constraint
            if(rooms[sol[cons.subject]].floor!=rooms[sol[cons.target]].floor){
                nRB+=1;
                softCons+="\t";
                softCons+=to_string(cons.id);
            }
            
        }
        else if(cons.type==9 && cons.hardness==false){//away from constraint
            if(rooms[sol[cons.subject]].floor==rooms[sol[cons.target]].floor){
                nRB+=1;
                softCons+="\t";
                softCons+=to_string(cons.id);
            }
        }
        
    }
    softCons=to_string(nRB)+softCons;
    myfile << softCons << endl;
    
    float overusedSpace=0;
    float underusedSpace=0;
    vector<float> usedSpace;
    for (int i=0;i<rooms.size();i++){
        usedSpace.push_back(0);
    }
    for(int i=0;i<sol.size();i++){
        usedSpace[sol[i]]+=entities[i].space;
    }
    for(int i=0;i<rooms.size();i++){
        if(rooms[i].space>usedSpace[i]){
            underusedSpace+=(rooms[i].space-usedSpace[i]);
        }
        else if(rooms[i].space<usedSpace[i]){
            overusedSpace+=(usedSpace[i]-rooms[i].space);
        }
    }
    myfile << underusedSpace+overusedSpace << "\t" << underusedSpace << "\t" << overusedSpace << endl << endl;
    for(int i=0;i<rooms.size();i++){
        myfile << i << "\t";
        if(rooms[i].space>usedSpace[i]){
            myfile << (rooms[i].space-usedSpace[i]) << "\t" << "0" << "\t";
        }
        else if(rooms[i].space<usedSpace[i]){
            myfile << "0" << "\t" << (usedSpace[i]-rooms[i].space) << "\t";
        }
        int nEh=0;
        string entidades="";
        for(int j=0;j<sol.size();j++){
            if(sol[j]==i){
                nEh++;
                entidades+=to_string(j);
                entidades+="\t";
            }
        }
        myfile << nEh << "\t" << entidades << endl;
    }
    
    myfile.close();
}
void writeSolution(){
    ofstream myfile;
    myfile.open ("/Users/carlos/Downloads/OSAP-Instancias/nott_data/output.txt",ios::app);
    for (int i:solution) {
        myfile << i << " ";
    }
    myfile << endl << objectiveFunction(solution) << endl << endl;
    myfile.close();
}

void forwardCheckingBT(){
    long unsigned int aux;
    for(int i=0;i<=entities.size();i++){
        
        if(i==entities.size()){//Si tengo una solucion, borra del dominio el ultimo valor instanseado y hace backtrack cronologico
            domain[i-1].erase(domain[i-1].begin());
            i-=2;
            //iterations--;
            if(objectiveFunction(solution)<objectiveFunction(bestSolution)){
                bestSolution=solution;
                writeSolution();
            }
            //if(iterations==0){
            //    return;
            //}
            continue;
        }
        if(domain[i].size()==0){//Si la entidad no tiene dominio, hace backtrack cronologico
            //cout<<"backtrack:"<<endl;
            if(i==0){
                return;
            }
            domain[i-1].erase(domain[i-1].begin());
            solution[i]=-1;
            for(int m=0;m<rooms.size();m++){
                domain[i].push_back(m);
            }
            i-=2;
            continue;
            
        }
        aux=domain[i].size();
        vector<int>::iterator it=domain[i].begin();
        int iterator=0;
        for(int j=0;j<aux;j++){
            if(!checkDomain(i, domain[i][iterator])){
                domain[i].erase(it);
                continue;
            }
            iterator++;
            it++;
        }

        if(domain[i].size()==0){//Si la entidad no tiene dominio, hace backtrack cronologico
            if(i==0){
                return;
            }
            domain[i-1].erase(domain[i-1].begin());
            solution[i]=-1;
            for(int m=0;m<rooms.size();m++){
                domain[i].push_back(m);
            }
            i-=2;
            continue;
            
        }
        solution[i]=domain[i][0];
    }
    
}

void forwardCheckingGBJ(){
    long unsigned int aux;
    for(int i=0;i<=entities.size();i++){
        
        if(i==entities.size()){//Si tengo una solucion, borra del dominio el ultimo valor instanseado y hace backtrack cronologico
            domain[i-1].erase(domain[i-1].begin());
            i-=2;
            iterations--;
            if(objectiveFunction(solution)<objectiveFunction(bestSolution)){
                bestSolution=solution;
                
            }
            if(iterations==0){
                return;
            }
            continue;
        }
        if(domain[i].size()==0){//Si la entidad no tiene dominio, hace backtrack cronologico
            if(i==0){
                return;
            }
            domain[i-1].erase(domain[i-1].begin());
            solution[i]=-1;
            for(int m=0;m<rooms.size();m++){
                domain[i].push_back(m);
            }
            i-=2;
            continue;
            
        }
        aux=domain[i].size();
        vector<int>::iterator it=domain[i].begin();
        int iterator=0;
        for(int j=0;j<aux;j++){
            if(!checkDomain(i, domain[i][iterator])){
                domain[i].erase(it);
                continue;
            }
            iterator++;
            it++;
        }
        
        if(domain[i].size()==0){//Si la entidad no tiene dominio, hace backtrack cronologico
            if(i==0){
                return;
            }
            domain[i-1].erase(domain[i-1].begin());
            solution[i]=-1;
            for(int m=0;m<rooms.size();m++){
                domain[i].push_back(m);
            }
            i-=2;
            continue;
            
        }
        solution[i]=domain[i][0];
    }
    
}


int main(int argc, const char * argv[]) {
    //cout << "Ingrese direccion de la instancia a leer:";
    string path="/Users/carlos/Downloads/OSAP-Instancias/nott_data/nott1d.txt";
    //cin >> path;
    readInstance(path);
    initSolution();
    initDomain();
    forwardCheckingBT();
    //generateOutput(bestSolution);
    //showSolution(bestSolution);
    //cout << objectiveFunction(bestSolution) << endl;
    return 0;
}
