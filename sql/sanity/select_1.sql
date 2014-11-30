-- NOTE:
-- This file performs sanity checks for INSERT and SELECT
-- Since you are testing INSERT using SELECT, you will have
-- to think carefully about the source of your bug.

-- Create relations
CREATE TABLE soaps(soapid integer, name char(32), 
                   network char(4), rating double);

-- Create index on all three datatypes
CREATE INDEX soaps (soapid);
CREATE INDEX soaps (rating);

-- insert with attributes in order
INSERT INTO soaps (soapid, name, network, rating) 
	VALUES (100, 'Gilmore Girls', 'CBS', 9.67);

-- insert with attributes out of order:
INSERT INTO soaps (network, rating, name, soapid)
	VALUES ('ABC', 3.35, 'Loving', 101);

-- Test file scan
SELECT * FROM soaps;

-- Test Index Select 
SELECT * FROM soaps WHERE soaps.soapid = 100;

-- Test Projections 
SELECT soaps.soapid, soaps.name, soaps.network FROM soaps WHERE soaps.soapid = 100;
SELECT soaps.soapid, soaps.network FROM soaps WHERE soaps.soapid = 101;


DROP TABLE soaps;
