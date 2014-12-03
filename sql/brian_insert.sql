--
-- test Insert with and without indices
--

-- create relations

CREATE TABLE inserttest(intcol integer, charcol1 char(20), charcol2 char(12), doublecol double);

-- Normal insert
INSERT INTO inserttest(intcol, charcol1, charcol2, doublecol)  VALUES (100, 'Some Text', 'Text', 6.0);

-- Omitting column
INSERT INTO inserttest(intcol, charcol1, charcol2)  VALUES (101, 'Some Text', 'Text');

-- Don't exist column
INSERT INTO inserttest(intcol, charcol1, charcol, doublecol)  VALUES (102, 'Some Text', 'Text', 6.0);

-- Too Many Columns
INSERT INTO inserttest(intcol, charcol1, charcol2, doublecol, doublecol)  VALUES (103, 'Some Text', 'Text', 6.0, 6.0);

-- Duplicated column name
INSERT INTO inserttest(intcol, charcol2, charcol2, doublecol)  VALUES (104, 'Some Text', 'Text', 6.0);

-- Double check
INSERT INTO inserttest(intcol, charcol1, charcol2, doublecol)  VALUES (105, 'Some Text', 'Text', 6);

-- Interger check
INSERT INTO inserttest(intcol, charcol1, charcol2, doublecol)  VALUES (106.23, 'Some Text', 'Text', 6.0);

-- Over Char limit
INSERT INTO inserttest(intcol, charcol1, charcol2, doublecol)  VALUES (107, 'Some Text', 'Thisisareallylongtext', 6.0);

-- Blank char
INSERT INTO inserttest(intcol, charcol1, charcol2, doublecol)  VALUES (108, 'Some Text', '', 6.0);

-- Out of order Test
INSERT INTO inserttest(intcol, charcol1, charcol2, doublecol)  VALUES (1, 'charcol1 1', 'charcol2 1', 1.0);
INSERT INTO inserttest(charcol1, charcol2, doublecol, intcol)  VALUES ('charcol1 2', 'charcol2 2', 2.0, 2);
INSERT INTO inserttest(charcol2, doublecol, intcol, charcol1)  VALUES ('charcol2 3', 3.0, 3, 'charcol1 3');
INSERT INTO inserttest(doublecol, intcol, charcol1, charcol2)  VALUES (4.0, 4, 'charcol1 4', 'charcol2 4');

	
SELECT * FROM inserttest;
DROP TABLE inserttest;
