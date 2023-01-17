

#include <iostream>
#include <occi.h>

using oracle::occi::Connection;
using oracle::occi::Environment;

using namespace oracle::occi;
using namespace std;

struct ShoppingCart
{
    int product_id;
    double price;
    int quantity;
};

void find_customer(int customer_id, int &found, Environment *env, Connection *conn);
void find_product(int product_id, double &price, Environment *env, Connection *conn);
void add_order(int customer_id, int &new_order_id, Environment *env, Connection *conn);

void add_order_item(int orderId, int itemId, int productId, int quantity, double price, Environment *env, Connection *conn);

void display_order(int orderId, Environment *env, Connection *conn);

void master_proc(int task, int parm1, Environment *env, Connection *conn);
int main()
{
    Environment *env = nullptr;
    Connection *conn = nullptr;
    string user = "dbs311_223ngg21";
    string pass = "45092389";
    string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";
    try
    {
        env = Environment::createEnvironment(Environment::DEFAULT);
        conn = env->createConnection(user, pass, constr);
        find_customer(1, 0, env, conn);
        find_product(1, 0, env, conn);
        add_order(1, 0, env, conn);
        add_order_item(1, 1, 1, 1, 1, env, conn);
        display_order(1, env, conn);
        master_proc(1, 1, env, conn);
    }
    catch (SQLException &sqlExcp)
    {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
    env->terminateConnection(conn);
    Environment::terminateEnvironment(env);
    return 0;
}
void find_customer(int customer_id, int &found, Environment *env, Connection *conn)
{
    Statement *stmt = conn->createStatement();
    ResultSet *rs = stmt->executeQuery("SELECT * FROM customers WHERE customer_id = " + to_string(customer_id));
    if (rs->next())
    {
        found = 1;
        cout << "Customer ID " << customer_id << " found." << endl;
    }
    else
    {
        found = 0;
        cout << "Customer ID " << customer_id << " not found." << endl;
    }
    conn->terminateStatement(stmt);
}
void find_product(int product_id, double &price, Environment *env, Connection *conn)
{
    Statement *stmt = conn->createStatement();
    ResultSet *rs = stmt->executeQuery("SELECT * FROM products WHERE product_id = " + to_string(product_id));
    if (rs->next())
    {
        price = rs->getDouble(3);
        cout << "Price of product ID " << product_id << " is " << price << endl;
    }
    else
    {
        price = 0;
        cout << "Product ID " << product_id << " not found." << endl;
    }
    conn->terminateStatement(stmt);
}
void add_order(int customer_id, int &new_order_id, Environment *env, Connection *conn)
{
    Statement *stmt = conn->createStatement();
    ResultSet *rs = stmt->executeQuery("SELECT MAX(order_id) FROM orders");
    if (rs->next())
    {
        new_order_id = rs->getInt(1) + 1;
        cout << "New order ID is " << new_order_id << endl;
    }
    else
    {
        new_order_id = 1;
        cout << "New order ID is " << new_order_id << endl;
    }
    stmt->executeUpdate("INSERT INTO orders VALUES (" + to_string(new_order_id) + ", " + to_string(customer_id) + ", 'Shipped', 56, SYSDATE)");
    cout << "Order ID " << new_order_id << " added for customer ID " << customer_id << endl;
    conn->terminateStatement(stmt);
}
void add_order_item(int orderId, int itemId, int productId, int quantity, double price, Environment *env, Connection *conn)
{
    Statement *stmt = conn->createStatement();
    stmt->executeUpdate("INSERT INTO order_items VALUES (" + to_string(orderId) + ", " + to_string(itemId) + ", " + to_string(productId) + ", " + to_string(quantity) + ", " + to_string(price) + ")");
    cout << "Order item added to order ID " << orderId << endl;
    conn->terminateStatement(stmt);
}
void display_order(int orderId, Environment *env, Connection *conn)
{
    Statement *stmt = conn->createStatement();
    ResultSet *rs = stmt->executeQuery("SELECT * FROM orders WHERE order_id = " + to_string(orderId));
    if (rs->next())
    {
        cout << "Order ID " << orderId << " found." << endl;
        cout << "Customer ID " << rs->getInt(2) << endl;
        rs = stmt->executeQuery("SELECT * FROM order_items WHERE order_id = " + to_string(orderId));
        double total = 0;
        while (rs->next())
        {
            cout << "Item ID " << rs->getInt(2) << endl;
            cout << "Product ID " << rs->getInt(3) << endl;
            cout << "Quantity " << rs->getInt(4) << endl;
            cout << "Price " << rs->getDouble(5) << endl;
            total += rs->getDouble(5);
        }
        cout << "Total price of order ID " << orderId << " is " << total << endl;
    }
    else
    {
        cout << "Order ID " << orderId << " not found." << endl;
    }
    conn->terminateStatement(stmt);
}
void master_proc(int task, int parm1, Environment *env, Connection *conn)
{
    if (task == 1)
    {
        int found;
        find_customer(parm1, found, env, conn);
    }
    else if (task == 2)
    {
        double price;
        find_product(parm1, price, env, conn);
    }
    else if (task == 3)
    {
        int new_order_id;
        add_order(parm1, new_order_id, env, conn);
    }
    else if (task == 4)
    {
        display_order(parm1, env, conn);
    }
    else
    {
        cout << "Invalid task." << endl;
    }
}