#include "Record.h"

int Record::getCopies() const {
    return copies;
}

int Record::getNumPurchases() const{
    return num_purchases;
}

void Record::addPurchase(){
    num_purchases++;
}

int Record::getAux() const{
    return aux;
}

void Record::setAux(int new_aux){
    aux = new_aux;
}

int Record::getHeight() const{
    return height;
}

void Record::setHeight(int new_height){
    height = new_height;
}

int Record::getRecordTypes() const{
    return record_types;
}

void Record::setRecordTypes(int types){
    record_types = types;
}

int Record::getColumn() const{
    return column;
}

void Record::setColumn(int col){
    column = col;
}

void Record::resetFather(){
    father = nullptr;
}

void Record::setFather(std::shared_ptr<Record> new_father){
    father = new_father;
}

std::shared_ptr<Record> Record::getFather() const {
    return father;
}

int Record::getId() const {
    return r_id;
}
