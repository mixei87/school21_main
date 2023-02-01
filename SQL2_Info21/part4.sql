DROP DATABASE IF EXISTS part4;
CREATE DATABASE part4 ENCODING 'UTF8';

CREATE TABLE IF NOT EXISTS users
(
    id   bigint PRIMARY KEY,
    name varchar,
    age  integer
);

CREATE TABLE IF NOT EXISTS tablename_1
(
    id   bigint PRIMARY KEY,
    name varchar,
    age  integer
);

CREATE TABLE IF NOT EXISTS tablename_2
(
    id   bigint PRIMARY KEY,
    name varchar,
    age  integer
);

CREATE TABLE IF NOT EXISTS tablename_3
(
    id   bigint PRIMARY KEY,
    name varchar,
    age  integer
);

CREATE TABLE IF NOT EXISTS not_tablename
(
    id   bigint PRIMARY KEY,
    name varchar,
    age  integer
);

DROP FUNCTION IF EXISTS fnc_add(IN a integer, IN b integer);
CREATE OR REPLACE FUNCTION fnc_add(IN a integer, IN b integer) RETURNS integer AS
$$
BEGIN
    RETURN a + b;
END
$$ LANGUAGE plpgsql;

DROP FUNCTION IF EXISTS fnc_sub(IN a integer, IN b integer);
CREATE OR REPLACE FUNCTION fnc_sub(IN a integer, IN b integer) RETURNS integer AS
$$
BEGIN
    RETURN a - b;
END
$$ LANGUAGE plpgsql;

DROP FUNCTION IF EXISTS fnc_without_param();
CREATE OR REPLACE FUNCTION fnc_without_param() RETURNS integer AS
$$
BEGIN
    RETURN 1;
END
$$ LANGUAGE plpgsql;

DROP FUNCTION IF EXISTS fnc_for_trigger();
CREATE OR REPLACE FUNCTION fnc_for_trigger() RETURNS trigger AS
$trg_audit$
BEGIN
    RETURN NULL;
END
$trg_audit$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS trg_audit ON users;
CREATE OR REPLACE TRIGGER trg_audit
    BEFORE INSERT
    ON users
    FOR EACH ROW
EXECUTE FUNCTION fnc_for_trigger();

-- ex1
-- Создать хранимую процедуру, которая, не уничтожая базу данных,
-- уничтожает все те таблицы текущей базы данных,
-- имена которых начинаются с фразы 'TableName'.

SELECT table_name
FROM information_schema.tables
WHERE table_schema = 'public';

CREATE OR REPLACE PROCEDURE pr_destroy_tables() AS
$$
DECLARE
    i varchar;
BEGIN
    FOR i IN SELECT table_name
             FROM information_schema.tables
             WHERE table_name LIKE 'tablename%'
               AND table_schema = 'public'
        LOOP
            EXECUTE FORMAT('drop table %I cascade', i);
        END LOOP;
END
$$ LANGUAGE plpgsql;

CALL pr_destroy_tables();

-- ex2
-- Создать хранимую процедуру с выходным параметром, которая выводит список имен и параметров
-- всех скалярных SQL функций пользователя в текущей базе данных.
-- Имена функций без параметров не выводить. Имена и список параметров должны выводиться в одну строку.
-- Выходной параметр возвращает количество найденных функций.

CREATE OR REPLACE PROCEDURE pr_list_scalar_functions(OUT count int, INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR SELECT ROUTINE_NAME, STRING_AGG(parameters.parameter_name, ',') AS parameters
                  FROM information_schema.routines
                           JOIN information_schema.parameters
                                ON routines.specific_name = parameters.specific_name
                  WHERE routines.specific_schema = 'public'
                    AND routine_type = 'FUNCTION'
                    AND parameters.parameter_name IS NOT NULL
                  GROUP BY ROUTINE_NAME;
    count = COUNT(*)
            FROM (SELECT ROUTINE_NAME, STRING_AGG(parameters.parameter_name, ',') AS parameters
                  FROM information_schema.routines
                           JOIN information_schema.parameters
                                ON routines.specific_name = parameters.specific_name
                  WHERE routines.specific_schema = 'public'
                    AND routine_type = 'FUNCTION'
                    AND parameters.parameter_name IS NOT NULL
                  GROUP BY ROUTINE_NAME) AS c;
END
$$ LANGUAGE plpgsql;

BEGIN;
CALL pr_list_scalar_functions(NULL);
FETCH ALL FROM "result";
END;

-- ex3
-- Создать хранимую процедуру с выходным параметром, которая уничтожает все SQL DML триггеры в текущей базе данных.
-- Выходной параметр возвращает количество уничтоженных триггеров.

SELECT trigger_name, event_object_table
FROM information_schema.triggers;

DROP PROCEDURE IF EXISTS pr_destroy_triggers(OUT number int);
CREATE OR REPLACE PROCEDURE pr_destroy_triggers(OUT number int) AS
$$
DECLARE
    r record;
BEGIN
    number = 0;
    FOR r IN
        SELECT trigger_name n, event_object_table t
        FROM information_schema.triggers
        LOOP
            EXECUTE FORMAT('drop trigger %I on %I cascade', r.n, r.t);
            number = number + 1;
        END LOOP;
END
$$ LANGUAGE plpgsql;

CALL pr_destroy_triggers(0);

-- ex4
--  Создать хранимую процедуру с входным параметром, которая выводит имена и описания типа объектов
--  (только хранимых процедур и скалярных функций), в тексте которых на языке SQL встречается строка,
--  задаваемая параметром процедуры.

DROP PROCEDURE if EXISTS pr_names_of_objects(IN varchar, INOUT curs refcursor);
CREATE OR REPLACE PROCEDURE pr_names_of_objects(IN varchar, INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR SELECT ROUTINE_NAME, routine_type
                  FROM information_schema.routines
                  WHERE ROUTINE_SCHEMA = 'public'
                    AND routine_type IN ('FUNCTION', 'PROCEDURE')
                    AND routine_definition LIKE CONCAT('%', $1, '%');
END
$$ LANGUAGE plpgsql;

BEGIN;
CALL pr_names_of_objects('DECLARE');
FETCH ALL FROM "result";
COMMIT;