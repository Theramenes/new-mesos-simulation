#include<stdio.h>
#include<iostream>

using namespace std;

// Base
class  BaseClass
{
private:
    /* data */
public:
    void offer();
    void addFramework();

protected:
    void generateOffers();
    virtual void _generateOffers();
};

void BaseClass::offer()
{
    addFramework();
    generateOffers();
    // addFramework();
    cout << endl;
}

void BaseClass::generateOffers()
{
    // cout << "Base Class : This is protected function in BaseClass." << endl;
    _generateOffers();
}

void BaseClass::_generateOffers()
{
    cout << "Base Class : _generateOffers." << endl;
}

void BaseClass::addFramework()
{
    cout << "Base Class : Adding framework." << endl;
    generateOffers();
}
// Base End

// Inherit
class InheritClass : public BaseClass
{
private:
    /* data */
public:
    // void addFramework();
    // void offer();
protected:
    void generateOffers();
    void _generateOffers();
};

// void InheritClass::offer()
// {
//     generateOffer();
//     addFramework();
// }

void InheritClass::generateOffers()
{
    // cout << "Inherit Class : This is hiding function in InheritClass." << endl;
    _generateOffers();
}

void InheritClass::_generateOffers()
{
    cout << "Inherit Class : _generateOffers." << endl;
}

// void InheritClass::addFramework()
// {
//     cout << "Inherit Class : Adding framework." << endl;
// }
// Inherit End

// int main()
// {
//     InheritClass inheritClass;
//     BaseClass baseClass;

//     baseClass.offer();
//     inheritClass.offer();
    
//     return 0;
// }