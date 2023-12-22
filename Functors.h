#ifndef DATA_STRUCTURES_2_WET_2_FUNCTORS_H

class CustomerById {
public:
    bool operator()(std::shared_ptr<Customer> u1, std::shared_ptr<Customer> u2){
        return u1->getId() < u2->getId();
    }
};


#define DATA_STRUCTURES_2_WET_2_FUNCTORS_H

#endif //DATA_STRUCTURES_2_WET_2_FUNCTORS_H
