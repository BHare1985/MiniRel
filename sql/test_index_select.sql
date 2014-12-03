CREATE TABLE COLORS(num integer, val integer, color char(80));

CREATE INDEX COLORS (val);

INSERT INTO COLORS (num, val, color) VALUES(1, 100, 'red');
INSERT INTO COLORS (num, val, color) VALUES(2, 100, 'orange');
INSERT INTO COLORS (num, val, color) VALUES(8, 400, 'periwinkle');
INSERT INTO COLORS (num, val, color) VALUES(3, 200, 'yellow');
INSERT INTO COLORS (num, val, color) VALUES(4, 200, 'green');
INSERT INTO COLORS (num, val, color) VALUES(5, 200, 'blue');
INSERT INTO COLORS (num, val, color) VALUES(6, 300, 'purple');
INSERT INTO COLORS (num, val, color) VALUES(7, 300, 'violet');

--Index Select:
SELECT COLORS.val, COLORS.color FROM COLORS WHERE COLORS.val = 100;   --Select many
SELECT COLORS.val, COLORS.color FROM COLORS WHERE COLORS.val = 300;   --Select many
SELECT COLORS.val, COLORS.color FROM COLORS WHERE COLORS.val = 400;   --Select 1
SELECT COLORS.val, COLORS.color FROM COLORS WHERE COLORS.val = 700;   --Select none
