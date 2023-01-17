
SET SERVEROUTPUT ON

CREATE OR REPLACE PROCEDURE find_customer (customer_id IN NUMBER, found OUT NUMBER) AS
BEGIN
SELECT COUNT(*) INTO found FROM customers WHERE customer_id = customer_id;
IF found = 0 THEN
DBMS_OUTPUT.PUT_LINE('Customer ID ' || customer_id || ' not found');
ELSE
DBMS_OUTPUT.PUT_LINE('Customer ID ' || customer_id || ' found');
END IF;
END;


CREATE OR REPLACE PROCEDURE find_product (product_id IN NUMBER, price OUT products.list_price%TYPE) AS
BEGIN
SELECT list_price INTO price FROM products WHERE product_id = product_id;
IF price = 0 THEN
DBMS_OUTPUT.PUT_LINE('Product ID ' || product_id || ' not found');
ELSE
DBMS_OUTPUT.PUT_LINE('Product ID ' || product_id || ' found');
END IF;
END;


CREATE OR REPLACE PROCEDURE add_order (customer_id IN NUMBER, new_order_id OUT NUMBER) AS
BEGIN
SELECT MAX(order_id) + 1 INTO new_order_id FROM orders;
INSERT INTO orders VALUES (new_order_id, customer_id, 'Shipped', 56, SYSDATE);
DBMS_OUTPUT.PUT_LINE('Order ID ' || new_order_id || ' added for customer ID ' || customer_id);
END;


CREATE OR REPLACE PROCEDURE add_order_item (orderId IN order_items.order_id%type,
 itemId IN order_items.item_id%type,
 productId IN order_items.product_id%type,
 quantity IN order_items.quantity%type,
 price IN order_items.unit_price%type) AS
BEGIN
INSERT INTO order_items VALUES (orderId, itemId, productId, quantity, price);
END;


CREATE OR REPLACE PROCEDURE display_order (orderId IN NUMBER) AS
BEGIN
SELECT * FROM orders WHERE order_id = orderId;
SELECT * FROM order_items WHERE order_id = orderId;
END;


CREATE OR REPLACE PROCEDURE master_proc (task IN NUMBER,
parm1 IN NUMBER) AS
BEGIN
IF task = 1 THEN
find_customer(parm1);
ELSIF task = 2 THEN
find_product(parm1);
ELSIF task = 3 THEN
add_order(parm1);
ELSIF task = 4 THEN
display_order(parm1);
END IF;
END;


CALL master_proc(1, 100);
CALL master_proc(1, 1000);
CALL master_proc(2, 100);
CALL master_proc(2, 1000);
CALL master_proc(3, 100);
CALL master_proc(3, 1000);
CALL add_order_item(10123, 1, 100, 1, 100);
CALL add_order_item(10123, 2, 100, 1, 100);
CALL add_order_item(10123, 3, 100, 1, 100);
CALL master_proc(4, 10123);
CALL master_proc(4, 10124);