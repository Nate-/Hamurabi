// Programmer:  Nathaniel Quan
// Description: Reproduces the Hamurabi text-based game in a 21st century context of restaurant
//              management.

#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;


//Global constants
const double FOOD_UNIT_COST = 30;                   //Food set at $30 a unit
const double CUSTOMERS_BILL = 100;                  //Every customer charged $100

//Function Prototypes
int random(int x, int y);
//Precondition: x is smaller value and y is bigger value
//Postcondition: returns an integer between x and y

void events(int& customers, int& employees, double& income, int& popularity, double& discount);
//Precondition: customers, employees, income and popularity all initialized to non-negative numbers
//Postcondition: calculates the effects of the events on parameters

void showResults(const int& customers, const int& employees, const double& income, const int& month , const int& food);
//Precondition: All parameters are already calculated as a result of current month decisions
//Post-condition: Outputs the results of the current month

void userInput(double& payroll, double& food_cost, double& marketing, int& food);
//Precondition: Variables to hold parameters already declared
//Post-condition: Variables filled by user input

int main(){

    //Variable Declarations
    int customers = 20, employees = 5, popularity = 100, month = 1, food = 50;
    double marketing_cost,  payroll, food_cost;
    double income = 1000;

    //Seed psuedo-random number generator
    srand(time(0));                 

    //Set decimal output
    cout.setf(ios::fixed);
    cout.precision(2);

    while(true){
        //Reset employee salary and hiring discounts
        double hiring_reduction = 0;
        double salary = employees * 10;
        
        //Logical check before event
        if (customers < 0) customers = 0;
        if (employees < 0) employees = 0;
        if (popularity < 0) popularity = 0;
        if (food < 0) food = 0;

        //Print monthly results
        showResults(customers, employees, income, month, food);
        
        //Exit condition
        if (income <= -500000){
            cout << "You go bankrupt" << endl;
            break;
        }

        //Prompt user for input
        userInput(payroll, food_cost, marketing_cost, food);
        
        //Food impacts
        if (food < customers && customers > 0 && food > 0){                             //Must have food to feed customers
            popularity -= random(10, 30);                                               //Or they won't spread word of your restaurant
            customers -= random(1, customers);                                          //And leave and never come back
        }

        //Calculate popularity
        if (marketing_cost > 0)
            if (marketing_cost > income)
                popularity = 100;                                                      //Gain popularity for advertising
            else
                popularity += random(1, 3);
        else
            popularity -= random(5, 10);                                               //Lose 5-10% popularity for not advertising

        if (popularity > 100) popularity = 100;
        else if (popularity < 0) popularity = 0;

        //Event
        events(customers, employees, income, popularity, hiring_reduction);

        //Calculate new income
        if (income > 0)
            income = income - payroll - food_cost - marketing_cost - income/5;          //Cost of running restaurant, including a random utility bill
        else
            income = income + payroll + food_cost + marketing_cost + income/5;          //Running deeper into debt
        
        if (customers > 5 * employees)
            income += 5 * employees * CUSTOMERS_BILL;                                   //Each employee can only serve 5 customers
        else
            income += customers * CUSTOMERS_BILL;                                       //All customers served

        //Calculate new food supply
        food = food - customers;

        //Calculate net employees gain/loss
        if (payroll >= salary && salary > 0)
            employees += random(1, payroll/10 - salary) + hiring_reduction;
        else
            employees -= random(1, employees/2);

        //Calculate new customers
        if (popularity >= 50)
            customers += random(5, popularity/2);
        else if (popularity >= 30)
            customers += random(1, 3);
        else
            customers -= random(1, abs(customers));

        month++;
    }

    return 0;
}


int random(int x, int y){
//Precondition: x is smaller value and y is bigger value
//Postcondition: returns an integer between x and y

    return x + rand() % y;
}


void events(int& customers, int& employees, double& income, int& popularity, double& discount){
//Precondition: customers, employees, income and popularity all initialized to non-negative numbers
//Postcondition: calculates the effects of the events on parameters
    
    //Generate random event (1-7)
    int thisEvent = random(1, 7);

    //Event Number and Effects:
    //  1 - Employees steal from you            Lose $50-250 per employee
    //  2 - Cockroach infestation               Lose up to 25% of customers & some popularity
    //  3 - Refrigeraton fails                  Lose up to 10% of income
    //  4 - Rat infestation                     Lose 50% of customers & some popularity
    //  5 - Best-food-in-state award            Gain up to 1 - 10 customers & some popularity
    //  6 - 5-star review                       Gain up to 5 - 25 of customers & some popularity
    //  7 - Competitor goes out of business     Employee hiring cost reduction (this amount of free workers)
    
    int custEffect = 0, popEffect = 0;
    double incEffect = 0;

    switch(thisEvent){
        case 1:
            incEffect = employees * random(50, 250) * -1;
            cout << "\tYour employees stole $" << fabs(incEffect) << " from you while you were away." << endl;
            break;
        case 2:
            if(customers > 4) custEffect = random(1, customers/4) * -1;
            else custEffect = -1;
            if (popularity > 0) popularity -= random(1, 15);
            cout << "\tYou have a cockroach infestation!!! " << endl;
            cout << "\tCustomers are outraged! " << abs(custEffect) << " customers storm out." << endl;
            break;
        case 3:
            if (income > 10) incEffect = random(income/10, income/3) / 100.0 * -1;
            else incEffect = random (1, 10);
            cout << "\tYour refrigeration fails due to a blackout. Your profits see a loss of $" << -1 * incEffect << endl;
            break;
        case 4:
            if(customers > 2) custEffect = random(1, customers/2) * -1;
            else custEffect = -1;
            if(popularity > 2) popularity -= random(1, popularity/2);
            else popularity -= 1;
            cout << "\tYou have a rat infestation!!! " << endl;
            cout << "\tCustomers are outraged! " << abs(custEffect) << " customers storm out." << endl;
            break;
        case 5:
            custEffect = random(1, 10);
            cout << "\t*** YOU WIN THE BEST-FOOD-IN-STATE AWARD *** " << endl;
            cout << "\t" << custEffect << " new customers gained." << endl;
            break;
        case 6:
            custEffect = random(5, 25);
            popularity += popularity * (random(10, 25) / 100);
            cout << "\t*** 5-STAR REVIEW FROM RENOWNED CRITIC *** " << endl;
            cout << "\t" << custEffect << " new customers gained." << endl;            
            break;
        case 7:
            discount = random(1, 9);
            cout << "\tA competitor goes out of business. Influx of workers in the market." << endl;
            cout << "\tYour payroll was more effectively spent!" << endl;
            break;
    } // end switch

    //Implement changes to customers and income
    customers += custEffect;
    income += incEffect;
    cout << endl;
}

void showResults(const int& customers, const int& employees, const double& income, const int& month, const int& food){
//Precondition: All parameters are already calculated as a result of current month decisions
//Post-condition: Outputs the results of the current month

    cout << "Month: " << month << endl;
    cout << "You have " << employees << " employees serving your " << customers << " customers." << endl;
    cout << "You have " << food << " units of food remaining. Your income is $ " << income << " for the month." << endl;
}

void userInput(double& payroll, double& food_cost, double& marketing, int& food){
//Precondition: Variables to hold parameters already declared
//Post-condition: Variables filled by user input

    int addlFood;

    cout << "  Total to spend on payroll: $";
    cin >> payroll;
    if (payroll < 0){
        cout << "You don't know how to pay your employees. They set your restaurant on fire." << endl;
        system("pause");
        exit(0);
    }

    cout << "  Units of food to buy ($" << FOOD_UNIT_COST << " a unit): ";
    cin >> addlFood;
    if (addlFood < 0){
        cout << "You can't count food. You get cheated out of your restaruant." << endl;
        system("pause");
        exit(0);
    }
    food += addlFood;
    food_cost = addlFood * FOOD_UNIT_COST;

    cout << "  Amount to spend on advertisement: $";
    cin >> marketing;
    if (marketing < 0){
        cout << "The IRS catch you for fraud and embezzlement. You go to jail and lose your restaurant." << endl;
        system("pause");
        exit(0);
    }


}