CREATE TABLE DA (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);
CREATE TABLE DB (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);

-- load tuples into the DA table
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (0, 11618, '00000 string record',  0.0);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (1, 15378, '00001 string record',  1.1);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (2, 17233, '00002 string record',  2.2);


-- load tuples into the DB table
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (0, 23798, '00000 string record',  0.0);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (1, 24529, '00001 string record',  1.1);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (2, 1440, '00002 string record',  2.2);


--
-- START JOIN TESTS
-- 

-- SNL TEST
SELECT * FROM DA, DB WHERE DA.serial < DB.serial; 

SELECT * FROM DB,DA WHERE DA.serial < DB.serial; 


SELECT DA.serial, DA.ikey, DA.filler FROM DA, DB WHERE DA.serial < DB.serial; 
SELECT DA.serial, DA.ikey, DA.filler FROM DA, DB WHERE DA.serial <= DB.serial; 
SELECT DA.serial, DA.ikey, DA.filler FROM DA, DB WHERE DA.serial > DB.serial;
SELECT DA.serial, DA.ikey, DA.filler FROM DA, DB WHERE DA.serial >= DB.serial;
SELECT DA.serial, DA.ikey, DA.filler FROM DA, DB WHERE DA.serial <> DB.serial;  


SELECT DB.serial, DB.ikey, DB.filler FROM DA, DB WHERE DA.serial < DB.serial; 
SELECT DB.serial, DB.ikey, DB.filler FROM DA, DB WHERE DA.serial <= DB.serial; 
SELECT DB.serial, DB.ikey, DB.filler FROM DA, DB WHERE DA.serial > DB.serial;
SELECT DB.serial, DB.ikey, DB.filler FROM DA, DB WHERE DA.serial >= DB.serial;
SELECT DB.serial, DB.ikey, DB.filler FROM DA, DB WHERE DA.serial <> DB.serial;  

SELECT DA.serial, DB.serial FROM DB,DA WHERE DA.serial < DB.serial;

DROP TABLE DA; 
DROP TABLE DB;
