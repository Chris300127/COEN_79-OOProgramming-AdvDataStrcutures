// FILE: database.cpp
// CLASS implemented: database (see database.h for documentation)

// INVARIANT for the database ADT:
//   1. The items in the database are stored in a dynamic array, where each entry of the array includes
//      a company name, a pointer to the head of the linked list of products, and a
//      a pointer to the tail of the linked list of products
//   2. The number of slots in the array of companies is stored in member varibale aloc_slots
//   3. The number of used slots of the array of companies is stored in member varibale used_slots


#ifndef DATABASE_CPP
#define DATABASE_CPP

#include "database.h"


//#define USEDEBUG

#ifdef USEDEBUG
#define Debug( x ) std::cout << x
#else
#define Debug( x )
#endif


namespace coen79_lab7
{
    
    database::database(const size_type& initial_capacity) {
        used_slots = 0;
        aloc_slots = initial_capacity;
        company_array = new company[aloc_slots];
    }
    
    
    database::database(const database &src) {
        Debug("Copy constructor..." << std::endl);

        // IMPLEMENTATION COMPLETED i think
        company_array = new company[src.aloc_slots];
        //creating copies of comapnies from src
        for(size_type i=0; i<src.used_slots; ++i){
            company_array[i] = src.company_array[i];
        }
        //update variables
        used_slots = src.used_slots;
        aloc_slots=src.aloc_slots;
        
    }
    
    
    database& database::operator= (const database &rhs) {
        Debug("Assignment operator..." << std::endl);

        // COMPLETE THE IMPLEMENTATION...
        delete[] company_array;  //deletes array before setting it to rhs
        //creating copies of comapnies from rhs
        company_array=new company[rhs.aloc_slots];
        for(size_type i =0; i<rhs.used_slots; ++i){
            company_array[i] = rhs.company_array[i];
        }
        //update variables 
        used_slots =rhs.used_slots;
        aloc_slots = rhs.aloc_slots;
        
        return *this;
    }
    
    
    database::~database() {
        // COMPLETE THE IMPLEMENTATION...
        for(size_type i = 0; i<used_slots; ++i){
            //deletes each company in company array
            company_array[i].~company();
        }
        delete[] company_array;
    }
    
    
    void database::reserve(size_type new_capacity) {
        Debug("Reserve..." << std::endl);

        if (new_capacity == aloc_slots)
            return; // The allocated memory is already the right size.
        
        if (new_capacity < used_slots)
            new_capacity = used_slots; // Canít allocate less than we are using.
        
        // IMPLEMENTATION COMPLETED
        aloc_slots = new_capacity;  //updates aloc slots
        company* temp = new company[aloc_slots];    //creates new larger array
        //copies over old data to new array
        for (size_type i=0; i<used_slots; ++i){
            temp[i] = company_array[i];
        }
        delete[] company_array;  //delete old array
        company_array = temp;
    }
    
    
    bool database::insert_company(const std::string &entry) {
        
        Debug("Insert company..." << std::endl);

        assert(entry.length() > 0);
        
        size_type pos = search_company(entry);
        
        // If you find a company that is false, because there are duplicates
        if (pos != COMPANY_NOT_FOUND) {
            return false;
        }

        // IMPLEMENTATION COMPLETED
        if (used_slots==aloc_slots){
            reserve(aloc_slots+10);  //adds 10 slots to array if no room
        }
        company temp = company(entry); //new company

        company_array[used_slots] = temp; //adds new company to next open spot in array
        ++used_slots;
        return true;
    }
    
    
    bool database::insert_item(const std::string &company, const std::string &product_name, const float &price) {
        Debug("Insert item..." << std::endl);

        assert(company.length() > 0 && product_name.length() > 0);

        // IMPLEMENTATION COMPLETED
        size_type pos = search_company(company);
        if (pos==COMPANY_NOT_FOUND){
            return false;  //product not inserted because company not found
        }
        else{
            company_array[pos].insert(product_name, price); //iserts product
            return true;
        }
        
        
    }
    
    
    bool database::erase_company(const std::string &company) {
        
        size_type company_index = search_company(company);
        
        // IMPLEMENTATION COMPLETED
        size_type pos = search_company(company);
        if (pos==COMPANY_NOT_FOUND){
            return false;
            //no company to delete
        }
        else{
            company_array[pos].~company(); //deletes found company
            //shifts rest if array up
            for(size_type i=pos; i<used_slots-1; ++i){
                company_array[i] = company_array[i+1];
            }
            --used_slots;
            return true;
        }
    }
    
    
    bool database::erase_item(const std::string& cName, const std::string& pName) {
        
        assert(cName.length() > 0 && pName.length() > 0);

        // IMPLEMENTATION COMPLETED
        size_type pos = search_company(cName);
        if (pos==COMPANY_NOT_FOUND){
            return false;
        }
        return company_array[pos].erase(pName);  //erases product from given company
    }
    
    
    
    database::size_type database::search_company(const std::string& company) {
        assert(company.length() > 0);

        // COMPLETE THE IMPLEMENTATION...
        for(size_type i=0; i<used_slots; ++i){
            if(company_array[i].get_name()==company){
                return i;  //position of found company
            }
        }
        return COMPANY_NOT_FOUND;
    }
    
    
    bool database::print_items_by_company(const std::string& cName) {
        assert(cName.length() > 0);

        size_type c_index = search_company(cName);
        
        if (c_index == COMPANY_NOT_FOUND) {
            return false;
        }
        
        std::cout << "Printing the products of " << cName << ":"<< std::endl;
        company_array[c_index].print_items();
        std::cout << std::endl;
        
        return true;
    }
    
    
    void database::print_companies() {
        
        std::cout << "Company List" << std::endl;
        for (int i = 0; i < used_slots; i++) {
            std::cout << "- " << company_array[i].get_name() << std::endl;
        }
    }
}

#endif
