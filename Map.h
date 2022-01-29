
#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <string>
#include <iostream>
using namespace std;

using KeyType = char;
using ValueType = int;

class Map
{
  public:
    Map();
    
    // copy constructor
    Map(const Map& other);
    
    // assignment operator
    Map& operator=(const Map& rhs);
    
    // destructor
    ~Map();
    
    bool empty() const;

    int size() const;

    bool insert(const KeyType& key, const ValueType& value);

    bool update(const KeyType& key, const ValueType& value);

    bool insertOrUpdate(const KeyType& key, const ValueType& value);

    bool erase(const KeyType& key);

    bool contains(const KeyType& key) const;
     
    bool get(const KeyType& key, ValueType& value) const;
    
    bool get(int i, KeyType& key, ValueType& value) const;

    void swap(Map& other);
    
    void dump() const;
    
  private:
    
    int m_size;
    
    struct Node {
        KeyType key;
        ValueType value;
        Node *next;
        Node *prev;
    };
    
    Node *head;
    Node *tail;
};

// non-member functions
bool merge(const Map& m1, const Map& m2, Map& result);
void reassign(const Map& m, Map& result);

#endif
