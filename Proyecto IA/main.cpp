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

int iterations=500000;//Solutions found until the algorithm stops
vector<int> solution;
vector<int> bestSolution;
vector<vector<int>> domain;
vector<Entity> entities;
vector<Room> rooms;
vector<Constraint> constraints;

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

/*void showSolution(){//Muestra la solucion encontrada
    for (vector<bool> vect : FutureSolution)
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
}*/
 
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
            /*cout << "("<< cons.subject;
            cout << solution[cons.subject];
            cout << solution[4];
            cout << ent;
            cout << room;
            cout << ")";*/
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
            int spaceUsed=0;
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

/*
void forwardChecking(){
    for(int i=0;i<entities.size();i++){
        for(int j=0;j<rooms.size();j++){
            vector<bool>::iterator it;
            for(it=domain[i][j].begin();it!=domain[i][j].end();++it){
                FutureSolution[i][j]=*it;
                if(!checkDomain(i,j)){
                    domain[i][j].erase(it);
                    --it;
                }
            }
            if(domain[i][j].size()==0){
                cout << "backtrack";
                return;
            }
            FutureSolution[i][j]=domain[i][j][0];
        }
    }
}
 */

int objectiveFunction(vector<int> sol){
    int overusedSpace=0;
    int underusedSpace=0;
    vector<int> usedSpace;
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
void writeSolution(){
    ofstream myfile;
    myfile.open ("/Users/carlos/Downloads/OSAP-Instancias/nott_data/solucion.txt",ios::app);
    for (int i:solution) {
        myfile << i << " ";
    }
    myfile << endl << objectiveFunction(solution) << endl << endl;
    myfile.close();
}

void forwardChecking(){
    long unsigned int aux;
    for(int i=0;i<=entities.size();i++){
        
        if(i==entities.size()){//Si tengo una solucion, borra del dominio el ultimo valor instanseado y hace backtrack cronologico
            domain[i-1].erase(domain[i-1].begin());
            i-=2;
            //cout<<"solucion:"<<endl;
            //writeSolution();
            iterations--;
            //cout << objectiveFunction(solution) << " " << objectiveFunction(bestSolution) << endl;
            if(objectiveFunction(solution)<objectiveFunction(bestSolution)){
                bestSolution=solution;
                
            }
            if(iterations==0){
                return;
            }
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
////////////////////////////////////// NO TOCAR ///////////////////////////////////////////////
        aux=domain[i].size();
        vector<int>::iterator it=domain[i].begin();
        //cout << "["<< aux <<"]";
        int iterator=0;
        for(int j=0;j<aux;j++){
            //cout <<"(" << i << domain[i][iterator]<<")";
            //cout << endl << endl;
            if(!checkDomain(i, domain[i][iterator])){
                domain[i].erase(it);
                continue;
            }
            iterator++;
            it++;
        }
////////////////////////////////////// NO TOCAR ///////////////////////////////////////////////

        //showSolution();
        //showDomain();
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
            //cout<<"solucion:"<<endl;
            continue;
            
        }
        solution[i]=domain[i][0];
    }
    
}

int main(int argc, const char * argv[]) {
    readInstance("/Users/carlos/Downloads/OSAP-Instancias/nott_data/nott1d.txt");
    initSolution();
    initDomain();
    forwardChecking();
    //showDomain();
    showSolution(bestSolution);
    cout << objectiveFunction(bestSolution) << endl;
    return 0;
}
