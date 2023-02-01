-- ex1
CREATE OR REPLACE PROCEDURE pr_add_p2p(checked_peer varchar, checking_peer varchar, task varchar, state check_status,
                                       "time" time) AS
$$
BEGIN
    IF state = 'Start' THEN
        EXECUTE FORMAT('INSERT INTO Checks VALUES (%L, %L, %L, %L)',
                       (SELECT MAX(id) + 1 FROM Checks), checked_peer, task, CURRENT_DATE);
        EXECUTE FORMAT('INSERT INTO P2P VALUES (%L, %L, %L, %L, %L)',
                       (SELECT MAX(id) + 1 FROM P2P),
                       (SELECT MAX(id) FROM Checks), checking_peer, state, time);
    ELSE
        EXECUTE FORMAT('INSERT INTO P2P VALUES (%L, %L, %L, %L, %L)',
                       (SELECT MAX(id) + 1 FROM P2P),
                       (SELECT MAX("check")
                        FROM P2P
                        WHERE P2P.state = 'Start'
                          AND P2P.checking_peer = pr_add_p2p.checking_peer),
                       checking_peer, state, time);
    END IF;
END
$$ LANGUAGE plpgsql;

-- test ex1
CALL pr_add_p2p('kwukong', 'rdexter', 'C4_s21_math', 'Start', '10:45');
CALL pr_add_p2p('kwukong', 'rdexter', 'C4_s21_math', 'Success', '11:15');

SELECT *
FROM checks
WHERE peer = 'kwukong'
  AND "date" = CURRENT_DATE;
SELECT *
FROM p2p
WHERE checking_peer = 'rdexter'
  AND "time" IN ('10:45', '11:15');

-- ex2
CREATE OR REPLACE PROCEDURE pr_add_verter(checked_peer varchar, checked_task varchar, state check_status,
                                          "time" time) AS
$$
DECLARE
    checks_max_id    bigint;
    p2p_max_check    bigint;
    verter_max_check bigint;
BEGIN
    SELECT MAX(id)
    INTO checks_max_id
    FROM checks
    WHERE checks.peer = checked_peer
      AND checks.task = checked_task;

    SELECT MAX("check")
    INTO p2p_max_check
    FROM p2p
    WHERE p2p.check = checks_max_id
      AND p2p.state = 'Success';

    SELECT MAX("check")
    INTO verter_max_check
    FROM verter
    WHERE verter.check = checks_max_id
      AND verter.state = 'Start';

    IF state = 'Start' AND p2p_max_check IS NOT NULL THEN
        EXECUTE FORMAT('INSERT INTO verter VALUES (%L, %L, %L, %L)',
                       (SELECT MAX(id) + 1 FROM verter),
                       p2p_max_check, state, time);

    ELSIF state IN ('Success', 'Failure') AND verter_max_check IS NOT NULL THEN
        EXECUTE FORMAT('INSERT INTO verter VALUES (%L, %L, %L, %L)',
                       (SELECT MAX(id) + 1 FROM verter),
                       verter_max_check, state, time);
    ELSE
        RAISE EXCEPTION 'ERROR: Wrong data in query';
    END IF;
END
$$ LANGUAGE plpgsql;

-- test ex2
CALL pr_add_verter('kwukong', 'C4_s21_math', 'Start', '11:45');
CALL pr_add_verter('kwukong', 'C4_s21_math', 'Success', '11:50');

SELECT *
FROM verter
WHERE "time" IN ('11:45', '11:50');

-- ex3
CREATE OR REPLACE FUNCTION fnc_trg_p2p_update_audit() RETURNS trigger AS
$p2p_audit$
BEGIN
    UPDATE transferred_points
    SET points_amount = points_amount + 1
    WHERE checking_peer = new.checking_peer
      AND checked_peer IN (SELECT peer
                           FROM checks
                                    JOIN p2p ON checks.id = p2p."check"
                           WHERE state = 'Start'
                             AND checks.id = new."check");
    RETURN new;
END
$p2p_audit$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER trg_p2p_update_audit
    AFTER INSERT
    ON p2p
    FOR EACH ROW
EXECUTE FUNCTION fnc_trg_p2p_update_audit();

-- test ex3
SELECT *
FROM transferred_points
WHERE checked_peer = 'kwukong'
  AND checking_peer = 'rdexter';

CALL pr_add_p2p('kwukong', 'rdexter', 'C4_s21_math', 'Start', '10:45');

-- ex4
CREATE OR REPLACE FUNCTION fnc_trg_xp_update_audit() RETURNS trigger AS
$xp_audit$
BEGIN
    IF (new.xp_amount > (SELECT max_xp
                         FROM tasks
                                  JOIN checks ON tasks.title = checks.task
                         WHERE checks.id = new."check")) OR
       (NOT EXISTS(SELECT * FROM p2p WHERE p2p."check" = new."check" AND state = 'Success')) OR
       (EXISTS(SELECT * FROM verter WHERE verter."check" = new."check" AND verter.state = 'Start') AND
        NOT EXISTS(SELECT * FROM verter WHERE verter."check" = new."check" AND verter.state = 'Success')) THEN
        RAISE EXCEPTION 'ERROR: Wrong XP amount';
    ELSE
        RETURN new;
    END IF;
END
$xp_audit$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER trg_xp_update_audit
    BEFORE INSERT
    ON xp
    FOR EACH ROW
EXECUTE FUNCTION fnc_trg_xp_update_audit();

-- test ex4
-- не завершенная проверка
INSERT INTO xp
VALUES ((SELECT MAX(id) + 1 FROM xp), 5, 200);

-- Failed проверка
INSERT INTO xp
VALUES ((SELECT MAX(id) + 1 FROM xp), 6, 200);

-- неверное количество ХР
INSERT INTO xp
VALUES ((SELECT MAX(id) + 1 FROM xp), 14, 500);

-- все ок
INSERT INTO xp
VALUES ((SELECT MAX(id) + 1 FROM xp), 14, 200);

SELECT *
  FROM xp
 WHERE id = (SELECT MAX(id) FROM xp);

DELETE
  FROM xp
 WHERE id = (SELECT MAX(id) FROM xp);