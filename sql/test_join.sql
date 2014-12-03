
CREATE TABLE DA2 (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);
CREATE TABLE DB2 (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);

INSERT INTO DA2 (serial, ikey, filler, dkey) VALUES (0, 11618, '00000 string record',  0.0);
INSERT INTO DA2 (serial, ikey, filler, dkey) VALUES (1, 15378, '00001 string record',  1.1);
INSERT INTO DA2 (serial, ikey, filler, dkey) VALUES (2, 17233, '00002 string record',  2.2);
INSERT INTO DA2 (serial, ikey, filler, dkey) VALUES (3, 19052, '00003 string record',  3.3);
INSERT INTO DA2 (serial, ikey, filler, dkey) VALUES (4, 24069, '00004 string record',  4.4);
INSERT INTO DA2 (serial, ikey, filler, dkey) VALUES (5, 17188, '00005 string record',  5.5);

INSERT INTO DB2 (serial, ikey, filler, dkey) VALUES (0, 23798, '00000 string record',  0.0);
INSERT INTO DB2 (serial, ikey, filler, dkey) VALUES (1, 24529, '00001 string record',  1.1);
INSERT INTO DB2 (serial, ikey, filler, dkey) VALUES (2, 1440, '00002 string record',  2.2);
INSERT INTO DB2 (serial, ikey, filler, dkey) VALUES (3, 20441, '00003 string record',  3.3);
INSERT INTO DB2 (serial, ikey, filler, dkey) VALUES (4, 8210, '00004 string record',  4.4);
INSERT INTO DB2 (serial, ikey, filler, dkey) VALUES (5, 31830, '00005 string record',  5.5);


-- equi-join, no indices
SELECT * FROM DA2, DB2 WHERE DA2.ikey = DB2.serial; -- SMJ

-- nonequi-join, no indices
SELECT * FROM DA2, DB2 WHERE DA2.ikey > DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey >= DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey < DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey <= DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey <> DB2.serial; -- SNL

CREATE INDEX DA2(ikey);

-- equi-join, left index
SELECT * FROM DA2, DB2 WHERE DA2.ikey = DB2.serial; -- INJ

-- nonequi-join, left index
SELECT * FROM DA2, DB2 WHERE DA2.ikey > DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey >= DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey < DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey <= DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey <> DB2.serial; -- SNL

DROP INDEX DA2(ikey);
CREATE INDEX DB2(serial);

-- equi-join, right index
SELECT * FROM DA2, DB2 WHERE DA2.ikey = DB2.serial; -- INJ

-- nonequi-join, right index
SELECT * FROM DA2, DB2 WHERE DA2.ikey > DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey >= DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey < DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey <= DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey <> DB2.serial; -- SNL

CREATE INDEX DA2(ikey);

-- equi-join, both indices
SELECT * FROM DA2, DB2 WHERE DA2.ikey = DB2.serial; -- INJ

-- nonequi-join, both indices
SELECT * FROM DA2, DB2 WHERE DA2.ikey > DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey >= DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey < DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey <= DB2.serial; -- SNL
SELECT * FROM DA2, DB2 WHERE DA2.ikey <> DB2.serial; -- SNL


DROP TABLE DA2;
DROP TABLE DB2;
