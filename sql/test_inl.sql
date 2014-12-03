CREATE TABLE COLORS(num integer, val integer, color char(80));
CREATE TABLE DOGNAMES(num integer, val integer, name char(80));

CREATE INDEX COLORS (val); 


INSERT INTO COLORS (num, val, color) VALUES(1, 100, 'red');
INSERT INTO COLORS (num, val, color) VALUES(2, 100, 'orange');
INSERT INTO COLORS (num, val, color) VALUES(8, 400, 'periwinkle');
INSERT INTO COLORS (num, val, color) VALUES(3, 200, 'yellow');
INSERT INTO COLORS (num, val, color) VALUES(4, 200, 'green');
INSERT INTO COLORS (num, val, color) VALUES(5, 200, 'blue');
INSERT INTO COLORS (num, val, color) VALUES(6, 300, 'purple');
INSERT INTO COLORS (num, val, color) VALUES(7, 300, 'violet');

INSERT INTO DOGNAMES (num, val, name) VALUES(1, 100, 'sparky');
INSERT INTO DOGNAMES (num, val, name) VALUES(2, 100, 'lilly');
INSERT INTO DOGNAMES (num, val, name) VALUES(3, 200, 'pal');
INSERT INTO DOGNAMES (num, val, name) VALUES(4, 300, 'spot');
INSERT INTO DOGNAMES (num, val, name) VALUES(6, 300, 'clifford');
INSERT INTO DOGNAMES (num, val, name) VALUES(7, 300, 'dash');

SELECT DOGNAMES.name, COLORS.color FROM DOGNAMES, COLORS WHERE DOGNAMES.val = COLORS.val; --one index test

CREATE INDEX DOGNAMES (val);

SELECT DOGNAMES.name, COLORS.color FROM DOGNAMES, COLORS WHERE DOGNAMES.val = COLORS.val; --two index test


