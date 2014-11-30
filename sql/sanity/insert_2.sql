-- NOTE:
-- This file performs sanity checks for INSERT and SELECT
-- Since you are testing INSERT using SELECT, you will have
-- to think carefully about the source of your bug.

-- Create relations
CREATE TABLE soaps(soapid integer, name char(32), 
                   network char(4), rating double);

-- Create index on all three datatypes
-- Should get an error for creating index on network (b/c of Char dataype)
CREATE INDEX soaps (soapid);
CREATE INDEX soaps (rating);
CREATE INDEX soaps (network);

-- insert with attributes in order
INSERT INTO soaps (soapid, name, network, rating) 
	VALUES (100, 'Gilmore Girls', 'CBS', 9.67);

-- Run a simple SELECT
SELECT * FROM soaps;

-- insert with attributes out of order:
INSERT INTO soaps (network, rating, name, soapid)
	VALUES ('ABC', 3.35, 'Loving', 101);

-- Run a simple SELECT
SELECT * FROM soaps;

-- Make sure relcat and attrcat are up to date
SELECT * FROM relcat;
SELECT * FROM attrcat;

DROP TABLE soaps;
