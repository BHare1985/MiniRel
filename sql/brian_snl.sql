-- Create the Datamations type of relations (note these tuples are smaller than that 
--  speficied in the Datamation benchmark).

-- NOTE:
-- 1. The queries are at the bottom of the file
-- 2. Be sure have tested SELECT and INSERT before testing the joins
-- 3. This test is by no means comprehensive 


CREATE TABLE DA (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);
CREATE TABLE DB (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);

-- load 100 tuples into the DA table
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (0, 11618, '00000 string record',  0.0);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (1, 15378, '00001 string record',  1.1);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (2, 17233, '00002 string record',  2.2);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (3, 19052, '00003 string record',  3.3);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (4, 24069, '00004 string record',  4.4);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (5, 17188, '00005 string record',  5.5);

-- load 102 tuples into the DB table
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (0, 23798, '00000 string record',  0.0);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (1, 24529, '00001 string record',  1.1);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (2, 1440, '00002 string record',  2.2);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (3, 20441, '00003 string record',  3.3);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (4, 8210, '00004 string record',  4.4);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (5, 31830, '00005 string record',  5.5);

--
-- START JOIN TESTS
-- 

-- SNL TEST
SELECT * FROM DA, DB WHERE DA.serial < DB.serial; 

SELECT DA.serial, DA.ikey, DA.filler FROM DA, DB WHERE DA.serial < DB.serial; 
SELECT DA.serial, DA.ikey, DA.filler FROM DA, DB WHERE DA.serial > DB.serial;
SELECT DA.serial, DA.ikey, DA.filler FROM DA, DB WHERE DA.serial <= DB.serial;  

SELECT DA.serial, DA.ikey, DA.filler FROM DA, DB WHERE DA.serial < 3; 

DROP TABLE DA; 
DROP TABLE DB;
