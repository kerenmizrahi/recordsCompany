#include "recordsCompany.h"

//-------------------- other ----------------------------------------

void print2D(std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> root, int space);

std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> findCustomer(std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> root, int id);

void delete_tree(std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>>& node);

int hash_function(int tableSize, int id);

AVLTree<std::shared_ptr<Customer>, CustomerById>* resize(AVLTree<std::shared_ptr<Customer>, CustomerById>* customers, int table_size);

void updateInorder(std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> root);

std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> findMember(std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> root, int id);

void add(std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> root, int id, double amount);

//-------------------- company methods -----------------------------

RecordsCompany::RecordsCompany() {
    customers = new AVLTree<std::shared_ptr<Customer>, CustomerById>[INITIAL_ARRAY_SIZE];
    for (int i = 0; i < INITIAL_ARRAY_SIZE; ++i) {
        customers[i].r = nullptr;
    }

    members = AVLTree<std::shared_ptr<Customer>, CustomerById>();
    size_of_table = INITIAL_ARRAY_SIZE;
    num_customers = 0;
    num_records = 0;
}

RecordsCompany::~RecordsCompany() {
    delete_tree(members.r);

    for (int i = 0; i < size_of_table; ++i) {
        delete_tree(customers[i].r);
    }
    delete[] customers;

    for (int i = 0; i < num_records; ++i) {
        records[i]->resetFather();
        records[i].reset();
    }
    delete[] records;
}

StatusType RecordsCompany::newMonth(int *records_stocks, int number_of_records){
    if (number_of_records < 0)
        return StatusType::INVALID_INPUT;

    try {
        // put 0 in all month_spendings (and prizes)
        updateInorder(members.r);

        // delete old record data
        for (int i = 0; i < num_records; ++i) {
            records[i]->resetFather();
            records[i].reset();
        }
        if (num_records > 0)
            delete[] records;

        // new records array
        num_records = number_of_records;
        records = new std::shared_ptr<Record>[number_of_records];
        for (int i = 0; i < number_of_records; ++i) {
            records[i] = std::shared_ptr<Record>(new Record(i, records_stocks[i]));
        }
    }
    catch (std::exception &e){
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType RecordsCompany::addCostumer(int c_id, int phone){

    if (phone < 0 || c_id < 0)
        return StatusType::INVALID_INPUT;
    try {
        //check if customer exists
        int index = hash_function(size_of_table, c_id);
        std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> customer = findCustomer(customers[index].r, c_id);
        if (customer != nullptr)
            return StatusType::ALREADY_EXISTS;

        // insert player to hash table
        std::shared_ptr<Customer> cstmr(new Customer(c_id, phone));
        Node<std::shared_ptr<Customer>, CustomerById> customer_to_add(cstmr);
        customers[index].r = customers[index].insert(customers[index].r, customer_to_add);
        num_customers++;

        // check factor of hash table
        if (num_customers / size_of_table == 1) {
            customers = resize(customers, size_of_table);
            size_of_table *= 2;
        }

    }catch (std::exception &e) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

Output_t<int> RecordsCompany::getPhone(int c_id){
    if (c_id < 0)
        return StatusType::INVALID_INPUT;

    //check if customer exists
    int index = hash_function(size_of_table, c_id);
    std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> customer = findCustomer(customers[index].r, c_id);
    if (customer == nullptr)
        return StatusType::DOESNT_EXISTS;

    return customer->value->getPhone();
}

StatusType RecordsCompany::makeMember(int c_id){
    if (c_id < 0)
        return StatusType::INVALID_INPUT;

    try {
        //check if customer exists
        int index = hash_function(size_of_table, c_id);
        std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> customer = findCustomer(customers[index].r, c_id);

        //if (c_id == 150)
        //    print2D(customers[index].r, 5);

        // customer doesn't exit
        if (customer == nullptr)
            return StatusType::DOESNT_EXISTS;

        // check if member exists
        std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> member = findCustomer(members.r, c_id);
        if (member != nullptr)
            return StatusType::ALREADY_EXISTS;

        // add new member
        customer->value->setMember();
        members.r = members.insert(members.r, customer->value);

    } catch(std::exception& e){
        return StatusType::ALLOCATION_ERROR;
    }

    //print2D(members.r, 5);

    return StatusType::SUCCESS;
}

Output_t<bool> RecordsCompany::isMember(int c_id){
    if (c_id < 0)
        return StatusType::INVALID_INPUT;

    //check if customer exists
    int index = hash_function(size_of_table, c_id);
    std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> customer = findCustomer(customers[index].r, c_id);
    // customer doesn't exit
    if (customer == nullptr)
        return StatusType::DOESNT_EXISTS;

    // check if member exists
    customer = findCustomer(members.r, c_id);
    if (customer != nullptr)
        return true;

    return false;
}

StatusType RecordsCompany::buyRecord(int c_id, int r_id) {
    if (c_id < 0 || r_id < 0)
        return StatusType::INVALID_INPUT;

    //check if customer exists
    int index = hash_function(size_of_table, c_id);
    std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> customer = findCustomer(customers[index].r, c_id);
    // customer doesn't exit
    if (customer == nullptr || r_id >= num_records)
        return StatusType::DOESNT_EXISTS;

    // check if member exists
    customer = findCustomer(members.r, c_id);
    if (customer != nullptr) {
        customer->value->addMonthSpendings(100 + records[r_id]->getNumPurchases());

        //std::cout << "id: " << c_id << std::endl;
        //print2D(members.r, 5);
    }
    records[r_id]->addPurchase();

    return StatusType::SUCCESS;
}

StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double  amount) {
    //change: third condition
    if (c_id1 < 0 || c_id2 < 0 || c_id1 > c_id2 || amount <= 0)
        return StatusType::INVALID_INPUT;

    //change: didn't handle the case where there are no members yet
    if (members.r == nullptr)
        return StatusType::SUCCESS;

    std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> min_node = members.minValueNode(members.r);
    std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> max_node = members.maxValueNode(members.r);
    int min_in_tree = min_node->value->getId();
    int max_in_tree = max_node->value->getId();

    //range not in tree
    if (min_in_tree > c_id2 || max_in_tree < c_id1)
        return StatusType::SUCCESS;

    std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> i1 = findMember(members.r, c_id1);
    std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> i2 = findMember(members.r, c_id2);

    //std::cout << "[" << c_id1 << ", " << c_id2 << "]" << std::endl;

    //only lower range not in tree
    if (c_id1 < min_in_tree && c_id2 <= max_in_tree){
        if (i2->value->getId() == c_id2)
            add(members.r, c_id2 - 1, amount);
        else
            add(members.r, i2->value->getId(), amount);

        //print2D(members.r, 5);
        //std::cout << "prize: " << amount << ", [" << min_in_tree << ", " << i2->value->getId() << "]" << std::endl;

    }

        //only upper range not in tree
    else if (c_id1 >= min_in_tree && c_id2 > max_in_tree){
        if (i1->value->getId() == c_id1)
            add(members.r, c_id1 - 1, -amount);
        else
            add(members.r, i1->value->getId(), -amount);

        add(members.r, max_in_tree, amount);

        //print2D(members.r, 5);
        //std::cout << "prize: " << amount << ", [" << i1->value->getId() << ", " << max_in_tree << "]" << std::endl;

    }

        //the whole tree is in range
    else if (c_id1 < min_in_tree && c_id2 > max_in_tree){
        add(members.r, max_in_tree, amount);

        //print2D(members.r, 5);
        //std::cout << "prize: " << amount << ", [" << min_in_tree << ", " << max_in_tree << "]" << std::endl;

    }

        //range in tree
    else {
        if (i1->value->getId() == c_id1)
            add(members.r, c_id1 - 1, -amount);
        else
            add(members.r, i1->value->getId(), -amount);

        if (i2->value->getId() == c_id2)
            add(members.r, c_id2 - 1, amount);
        else
            add(members.r, i2->value->getId(), amount);

        //print2D(members.r, 5);
        //std::cout << "prize: " << amount << ", [" << i1->value->getId() << ", " << i2->value->getId() << "]" << std::endl;

    }

    return StatusType::SUCCESS;
}

Output_t<double> RecordsCompany::getExpenses(int c_id) {
    if (c_id < 0)
        return StatusType::INVALID_INPUT;

    std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> customer = findCustomer(members.r, c_id);
    if (customer == nullptr)
        return StatusType::DOESNT_EXISTS;
    //change: made the second argument of getSum to be a Node, not T val
    double sum = members.getSum(members.r, customer->value);

    //std::cout << "id: " << c_id << " sum: " << sum << "spendings: " << customer->value->getMonthSpendings() << std::endl;
    return (customer->value->getMonthSpendings() - sum);
}

StatusType RecordsCompany::putOnTop(int r_id1, int r_id2) {
    if (r_id1 < 0 || r_id2 < 0)
        return StatusType::INVALID_INPUT;
    if (r_id1 >= num_records || r_id2 >= num_records)
        return StatusType::DOESNT_EXISTS;

    if (r_id1 == r_id2)
        return StatusType::FAILURE;

    int column1 = 0, height1 = 0, column2 = 0, height2 = 0;
    std::shared_ptr<Record> r1 = records[r_id1];
    while (r1->getFather() != nullptr) {//find root and calculate sum of aux
        r1 = r1->getFather();
    }

    std::shared_ptr<Record> r2 = records[r_id2];
    while (r2->getFather() != nullptr) {//find root and calculate sum of aux
        r2 = r2->getFather();
    }

    if (r1->getId() == r2->getId())
        return StatusType::FAILURE;

    getPlace(r_id1, &column1, &height1);
    getPlace(r_id2, &column2, &height2);

    if (r1->getRecordTypes() > r2->getRecordTypes())  // r1 is the root of new heap
    {
        r1->setHeight(r1->getHeight() + r2->getHeight());
        r1->setRecordTypes(r1->getRecordTypes() + r2->getRecordTypes());
        r1->setAux(r1->getAux() + r2->getHeight());
        r1->setColumn(r2->getColumn());

        r2->setHeight(0);
        r2->setRecordTypes(0);
        r2->setAux(r2->getAux() - r1->getAux());
        r2->setFather(r1);
        r2->setColumn(0);
    }
    else {
        //change: we added the NEW height of the whole stack instead of only the bottom stack, so I switched the order
        int temp_height = r1->getHeight(), temp_types = r1->getRecordTypes();
        r1->setHeight(0);
        r1->setRecordTypes(0);
        r1->setAux(r1->getAux() + r2->getHeight() - r2->getAux());
        r1->setFather(r2);
        r1->setColumn(0);

        r2->setHeight(temp_height + r2->getHeight());
        r2->setRecordTypes(temp_types + r2->getRecordTypes());
    }

    return StatusType::SUCCESS;
}

StatusType RecordsCompany::getPlace(int r_id, int *column, int *height){
    if (r_id < 0 || column == nullptr || height == nullptr)
        return StatusType::INVALID_INPUT;
    if (r_id >= num_records)
        return StatusType::DOESNT_EXISTS;

    int sum = 0;
    int to_sub = 0;
    std::shared_ptr<Record> path_record = records[r_id];
    // summing all aux from r_id to root
    if (records[r_id]->getFather() == nullptr) // if we are the root
        sum = records[r_id]->getAux();

    else {  // if r_id is not the root
        while (path_record->getFather() != nullptr) {//find root and calculate sum of aux
            sum += path_record->getAux();
            path_record = path_record->getFather();
        }

        std::shared_ptr<Record> root = path_record;
        path_record = records[r_id];
        std::shared_ptr<Record> temp_father = records[r_id]->getFather();

        while (temp_father != root && temp_father != nullptr) {//shrinking ---------------------- while != root instead of nullptr
            temp_father = path_record->getFather();
            int temp_aux = path_record->getAux();
            ///path_record->setAux(path_record->getAux() - to_sub);
            path_record->setAux(sum - to_sub);
            path_record->setFather(root);
            path_record = temp_father;
            to_sub += temp_aux;
        }

        path_record = root;
        sum += path_record->getAux();
    }
    *height = sum;
    *column = path_record->getColumn();
    return StatusType::SUCCESS;
}

// statements -------------------------------------------------------

void delete_tree(std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>>& node) {
    if (node) {
        delete_tree(node->left);
        delete_tree(node->right);
        node->right = nullptr;
        node->left = nullptr;
        node->value = nullptr;
        node = nullptr;
    }
}

AVLTree<std::shared_ptr<Customer>, CustomerById>* resize(AVLTree<std::shared_ptr<Customer>, CustomerById>* customers, int table_size) {

    AVLTree<std::shared_ptr<Customer>, CustomerById> *bigger = new AVLTree<std::shared_ptr<Customer>, CustomerById>[2 * table_size];
    for (int i = 0; i < 2*table_size; ++i) { //initialize bigger table
        bigger[i].r = nullptr;
    }

    for (int i = 0; i < table_size; ++i) {
        while (customers[i].r != nullptr) { //to empty current cell
            std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> root = customers[i].r; //root of tree
            int index = hash_function(2*table_size, root->value->getId());//new index for root
            bigger[index].r = bigger[index].insert(bigger[index].r, root->value); //add the root to the bigger array in the new index
            customers[i].r = customers[i].deleteNode(root, root->value); //remove the root from the tree
        }
    }

    delete[] customers;
    return bigger;
}

int hash_function(int tableSize, int id){
    return id % tableSize;
}

void updateInorder(std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> root) {
    if (root == nullptr)
        return;

    std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> node = root;
    std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> pre;

    while (node != nullptr) {
        if (node->left == nullptr) {
            node->value->setMonthSpendings(0);
            node->subTree = 0;
            node = node->right;
        }
        else {
            pre = node->left;
            while (pre->right != nullptr && pre->right != node)
                pre = pre->right;

            if (pre->right == nullptr) {
                pre->right = node;
                node = node->left;
            }
            else {
                pre->right = nullptr;
                node->value->setMonthSpendings(0);
                node->subTree = 0;
                node = node->right;
            }
        }
    }
}

std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> findCustomer(std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> root, int id) {
    if (root == nullptr) {
        return nullptr;
    }

    if (root -> value -> getId() < id) { //id > root id
        return findCustomer(root->right, id);
    }
    else if (root ->  value -> getId() > id) {  // val > root -> value
        return findCustomer(root->left, id);
    }
    else { //duplicate values
        return root;
    }
}

std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> findMember(std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> root, int id) {
    if (root == nullptr)
        return nullptr;

    std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> node = root;

    while (root != nullptr) {
        if (root->left == nullptr && root->right == nullptr){
            if (root->value->getId() < id)
                return root;
        }

        if (id < root->value->getId()) { //val < root -> value
            root = root->left;
        }
            // If the key is greater than the root's key, then it lies in right subtree
        else if (root->value->getId() < id) { //val > root -> value
            node = root;
            root = root->right;
        }
            // if key is same as root's key, then This is the node to be deleted
        else {
            return root;
        }
    }

    return node;
}

void add(std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> root, int id, double amount) {
    bool right_seq = false;

    while (root != nullptr) {
        if (id < root->value->getId()) { //val < root -> value >>> left turn
            if (right_seq == true)
                root->subTree -= amount;
            root = root->left;
            right_seq = false;
        }
            // If the key is greater than the root's key, then it lies in right subtree
        else if (root->value->getId() < id) { //val > root -> value >>> right turn
            if (right_seq == false)
                root->subTree += amount;

            right_seq = true;
            root = root->right;
        }
            // if key is same as root's key, then This is the node to be deleted
        else {
            // treat as right turn
            if (right_seq == false)
                root->subTree += amount;

            right_seq = true;
            root = root->right;

            if (root != nullptr) {
                if (right_seq == true)
                    root->subTree -= amount;
            }

            return;
        }
    }
}

void print2D(std::shared_ptr<Node<std::shared_ptr<Customer>, CustomerById>> root, int space) {
    if (root == nullptr) // Base case  1
        return;

    space += SPACE; // Increase distance between levels   2
    print2D(root->right, space); // Process right child first 3
    std::cout << std::endl;

    for (int i = SPACE; i < space; i++) // 5
        std::cout << " "; // 5.1

    std::cout << root->value->getId() << ", - " << root->subTree << ", + " << root->value->getMonthSpendings() << "\n"; // 6
    print2D(root->left, space); // Process left child  7
}