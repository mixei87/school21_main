-- Success checks
CREATE OR REPLACE VIEW v_success_check AS
SELECT checks.id, peer, task, date
FROM checks
         JOIN p2p ON checks.id = p2p."check" AND p2p.state = 'Success'
WHERE checks.id NOT IN (SELECT "check"
                        FROM verter
                        WHERE state = 'Start'
                        EXCEPT
                        SELECT "check"
                        FROM verter
                        WHERE state = 'Success');

-- ex01
-- Написать функцию, возвращающую таблицу TransferredPoints в более человекочитаемом виде
CREATE OR REPLACE FUNCTION fnc_transferred_points()
    RETURNS table
            (
                Peer1        varchar,
                Peer2        varchar,
                PointsAmount bigint
            )
AS
$$
BEGIN
    RETURN QUERY SELECT peers.p1 AS peer_1, peers.p2 AS peer_2, SUM(peers.pa) AS p_a
                 FROM (WITH tp1 AS (SELECT checking_peer p1, checked_peer p2, points_amount pa
                                    FROM transferred_points
                                    WHERE checking_peer > checked_peer),
                            tp2 AS (SELECT checked_peer p1, checking_peer p2, -points_amount pa
                                    FROM transferred_points
                                    WHERE checked_peer > checking_peer)
                       SELECT *
                       FROM tp1
                       UNION
                       SELECT *
                       FROM tp2) AS peers
                 GROUP BY peers.p1, peers.p2
                 ORDER BY peer_1, peer_2;
END
$$ LANGUAGE plpgsql;

-- call ex01
SELECT *
FROM fnc_transferred_points();

-- ex02
-- Написать функцию, которая возвращает таблицу вида: ник пользователя, название проверенного задания,
-- кол-во полученного XP
CREATE OR REPLACE FUNCTION fnc_peer_task_xp()
    RETURNS table
            (
                Peer varchar,
                Task varchar,
                XP   integer
            )
AS
$$
BEGIN
    RETURN QUERY SELECT Checks.peer  AS Peer,
                        Checks.task  AS Task,
                        XP.xp_amount AS XP
                 FROM XP
                          JOIN Checks ON Checks.id = XP."check"
                 ORDER BY Peer, Task;
END
$$ LANGUAGE plpgsql;

-- call ex02
SELECT *
FROM fnc_peer_task_xp();

-- ex03
-- Написать функцию, определяющую пиров, которые не выходили из кампуса в течение всего дня
CREATE OR REPLACE FUNCTION fnc_left_campus(pdate date)
    RETURNS TABLE
            (
                peer varchar
            )
AS
$$
WITH cte AS (SELECT COUNT(peer) AS count, peer FROM time_tracking WHERE "date" = pdate GROUP BY peer)
SELECT peer
FROM cte
WHERE count = 2;
$$ LANGUAGE SQL;

-- call ex03
SELECT *
FROM fnc_left_campus('2022-06-23');

-- ex04
-- Find the percentage of successful and unsuccessful checks for all time
CREATE OR REPLACE PROCEDURE percent_checks(INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR SELECT ROUND(100 / (SUM(success.state) + SUM(failure.state)) *
                               SUM(success.state)) AS SuccessfulChecks,
                         ROUND(100 / (SUM(success.state) + SUM(failure.state)) *
                               SUM(failure.state)) AS UnsuccessfulChecks
                  FROM (SELECT COUNT(state) AS state
                        FROM p2p
                        WHERE state = 'Success'
                        UNION ALL
                        SELECT COUNT(state) AS state
                        FROM verter
                        WHERE state = 'Success') AS success,
                       (SELECT COUNT(state) AS state
                        FROM p2p
                        WHERE state = 'Failure'
                        UNION ALL
                        SELECT COUNT(state) AS state
                        FROM verter
                        WHERE state = 'Failure') AS failure;
END
$$ LANGUAGE plpgsql;

-- call ex04
BEGIN;
CALL percent_checks();
FETCH ALL FROM "result";
END;

-- ex05
-- Посчитать изменение в количестве пир поинтов каждого пира по таблице TransferredPoints
CREATE OR REPLACE PROCEDURE delta_points_for_peer(INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR SELECT Peer, SUM(PointsChange) AS PointsChange
                  FROM (SELECT checking_peer AS Peer, points_amount AS PointsChange
                        FROM transferred_points
                        UNION
                        SELECT checked_peer AS Peer, points_amount * -1 AS PointsChange
                        FROM transferred_points) unique_transferred_points
                  GROUP BY Peer
                  ORDER BY PointsChange DESC;
END
$$ LANGUAGE plpgsql;

-- call ex05
BEGIN;
CALL delta_points_for_peer();
FETCH ALL FROM "result";
END;

--ex06
-- Посчитать изменение в количестве пир поинтов каждого пира по таблице, возвращаемой первой функцией из Part 3
CREATE OR REPLACE PROCEDURE peerpoints_changes(INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR WITH cte AS (SELECT *
                               FROM fnc_transferred_points()
                               UNION
                               SELECT Peer2, Peer1, -PointsAmount
                               FROM fnc_transferred_points()
                               ORDER BY PointsAmount DESC)
                  SELECT Peer1 AS peer, SUM(PointsAmount) AS PointsChange
                  FROM cte
                  GROUP BY Peer1
                  ORDER BY PointsChange DESC;
END
$$ LANGUAGE plpgsql;

-- call ex06
BEGIN;
CALL peerpoints_changes();
FETCH ALL FROM "result";
END;

-- ex7
-- Определить самое часто проверяемое задание за каждый день
CREATE OR REPLACE PROCEDURE pr_most_checked(INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR WITH cte AS (SELECT COUNT(*) AS count, task, "date" FROM checks GROUP BY "date", task),
                       cte2 AS (SELECT "date", MAX(count) AS max FROM cte GROUP BY "date")
                  SELECT cte."date" AS day, task
                  FROM cte
                           JOIN cte2 ON cte."date" = cte2."date" AND cte.count = cte2.max;
END
$$ LANGUAGE plpgsql;

BEGIN;
CALL pr_most_checked();
FETCH ALL FROM "result";
COMMIT;

-- ex08
-- Определить длительность последней P2P проверки
CREATE OR REPLACE PROCEDURE duration_last_p2p(INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR SELECT (p2.time - p1.time)::time AS duration_p2p
                  FROM p2p p1
                           JOIN p2p p2 ON p1."check" = p2."check" AND p1.state = 'Start' AND p2.state <> 'Start'
                           JOIN checks ON p1."check" = checks.id
                  ORDER BY date DESC, p2.time
                  LIMIT 1;
END
$$ LANGUAGE plpgsql;

-- call ex08
BEGIN;
CALL duration_last_p2p();
FETCH ALL FROM "result";
END;

-- ex09
-- Найти всех пиров, выполнивших весь заданный блок задач и дату завершения последнего задания
CREATE OR REPLACE PROCEDURE pr_completed_block(IN block varchar, INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR WITH cte AS (SELECT MAX(title) AS task
                               FROM tasks
                               WHERE title IN
                                     (SELECT UNNEST(REGEXP_MATCHES(title, CONCAT('(', block, '\d.*)'))) FROM tasks))
                  SELECT peer, "date" AS day
                  FROM v_success_check
                           JOIN cte ON v_success_check.task = cte.task
                  ORDER BY day;
END
$$ LANGUAGE plpgsql;

-- call ex09
BEGIN;
CALL pr_completed_block('C');
FETCH ALL FROM "result";
COMMIT;

--ex10
-- Определить, к какому пиру стоит идти на проверку каждому обучающемуся
CREATE OR REPLACE PROCEDURE check_recommend(INOUT curs refcursor = 'result')
AS
$$
BEGIN
    OPEN curs FOR SELECT a.nickname AS peer, a.recommended_peer AS recommendedpeer
                  FROM (SELECT peers.nickname, rec.recommended_peer, COUNT(rec.recommended_peer) AS num
                        FROM peers
                                 JOIN friends
                                      ON peers.nickname = friends.peer1 OR peers.nickname = friends.peer2
                                 JOIN recommendations AS rec
                                      ON (friends.peer2 = rec.peer AND friends.peer2 != peers.nickname) OR
                                         (friends.peer1 = rec.peer AND friends.peer1 != peers.nickname)
                        WHERE peers.nickname != rec.recommended_peer
                        GROUP BY peers.nickname, rec.recommended_peer) AS a
                  WHERE a.num = (SELECT MAX(b.num)
                                 FROM (SELECT COUNT(rec.recommended_peer) AS num
                                       FROM peers
                                                JOIN friends
                                                     ON peers.nickname = friends.peer1 OR peers.nickname = friends.peer2
                                                JOIN recommendations AS rec
                                                     ON (friends.peer2 = rec.peer AND friends.peer2 != peers.nickname) OR
                                                        (friends.peer1 = rec.peer AND friends.peer1 != peers.nickname)
                                       WHERE peers.nickname != rec.recommended_peer
                                         AND a.nickname = peers.nickname
                                       GROUP BY peers.nickname, rec.recommended_peer) AS b);
END;
$$ LANGUAGE plpgsql;

-- call ex10
BEGIN;
CALL check_recommend();
FETCH ALL IN "result";
END;

-- ex11
-- Определить процент пиров, которые: Приступили только к блоку 1, Приступили только к блоку 2,
-- Приступили к обоим, Не приступили ни к одному
CREATE OR REPLACE FUNCTION peers_started_block(block varchar)
    RETURNS table
            (
                peer varchar
            )
AS
$$
DECLARE
    mask_block text := '^' || block || '[0-9]+_';
BEGIN
    RETURN QUERY (SELECT DISTINCT checks.peer FROM checks WHERE task ~ mask_block);
END
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION percent(val1 bigint, val2 bigint) RETURNS varchar AS
$$
BEGIN
    RETURN TO_CHAR(val1::float * 100 / val2, 'fm90D0%');
END
$$ LANGUAGE plpgsql;

CREATE OR REPLACE PROCEDURE percent_peers_on_2_blocks(IN block1 varchar, IN block2 varchar,
                                                      INOUT curs refcursor = 'result') AS
$$
DECLARE
    count_all_peers                    bigint := (SELECT COUNT(*)
                                                  FROM peers);
    count_peers_block1                 bigint;
    count_peers_block2                 bigint;
    count_peers_on_2_blocks            bigint;
    count_peers_didnt_started_2_blocks bigint;
BEGIN
    DROP TABLE IF EXISTS peers_started_block1;
    CREATE TEMP TABLE peers_started_block1 AS
            (SELECT * FROM peers_started_block(block1));
    DROP TABLE IF EXISTS peers_started_block2;
    CREATE TEMP TABLE peers_started_block2 AS
            (SELECT * FROM peers_started_block(block2));
    count_peers_block1 := (SELECT COUNT(*) FROM peers_started_block1);
    count_peers_block2 := (SELECT COUNT(*) FROM peers_started_block2);
    count_peers_on_2_blocks := (SELECT COUNT(*)
                                FROM peers_started_block1 p1
                                         JOIN peers_started_block2 p2 ON p1.peer = p2.peer);
    count_peers_didnt_started_2_blocks :=
            (SELECT COUNT(*)
             FROM (SELECT nickname
                   FROM peers
                   EXCEPT
                   SELECT *
                   FROM peers_started_block1
                   EXCEPT
                   SELECT *
                   FROM peers_started_block2) peers_didnt_started_2_blocks);
    OPEN curs FOR SELECT percent(count_peers_block1, count_all_peers)                 AS "StartedBlock1",
                         percent(count_peers_block2, count_all_peers)                 AS "StartedBlock2",
                         percent(count_peers_on_2_blocks, count_all_peers)            AS "StartedBothBlocks",
                         percent(count_peers_didnt_started_2_blocks, count_all_peers) AS "DidntStartAnyBlock";
END
$$ LANGUAGE plpgsql;

--call1 ex11
BEGIN;
CALL percent_peers_on_2_blocks('C', 'DO');
FETCH ALL FROM "result";
END;

--call2 ex11
BEGIN;
CALL percent_peers_on_2_blocks('CPP', 'A');
FETCH ALL FROM "result";
END;

--call3 ex11
BEGIN;
CALL percent_peers_on_2_blocks('A', 'B');
FETCH ALL FROM "result";
END;

-- ex12
-- Определить N пиров с наибольшим числом друзей
CREATE OR REPLACE PROCEDURE pr_number_of_friends(IN n integer, INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR WITH cte AS (SELECT id, peer1 AS peer FROM friends UNION SELECT id, peer2 AS peer FROM friends)
                  SELECT peer, COUNT(peer) AS FriendsCount
                  FROM cte
                  GROUP BY peer
                  ORDER BY FriendsCount DESC
                  LIMIT n;
END
$$ LANGUAGE plpgsql;

-- call ex12
BEGIN;
CALL pr_number_of_friends(4);
FETCH ALL FROM "result";
COMMIT;

--ex13
-- Определить процент пиров, которые когда-либо успешно проходили проверку в свой день рождения
CREATE OR REPLACE PROCEDURE birthday_checks(INOUT curs refcursor = 'result') AS
$$
DECLARE
    total              numeric := (SELECT COUNT(*)
                                   FROM peers
                                            JOIN checks
                                                 ON peers.nickname = checks.peer
                                            JOIN p2p
                                                 ON checks.id = p2p.check
                                   WHERE EXTRACT(DAY FROM peers.birthday) = EXTRACT(DAY FROM checks.date)
                                     AND EXTRACT(MONTH FROM peers.birthday) = EXTRACT(MONTH FROM checks.date)
                                     AND state = 'Start');
    SuccessfulChecks   numeric := ROUND((SELECT COUNT(*)
                                         FROM peers
                                                  JOIN checks
                                                       ON peers.nickname = checks.peer
                                                  JOIN p2p
                                                       ON checks.id = p2p.check
                                         WHERE EXTRACT(DAY FROM peers.birthday) = EXTRACT(DAY FROM checks.date)
                                           AND EXTRACT(MONTH FROM peers.birthday) = EXTRACT(MONTH FROM checks.date)
                                           AND state = 'Success') * 100 / total, 2);
    UnsuccessfulChecks numeric := ROUND((SELECT COUNT(*)
                                         FROM peers
                                                  JOIN checks
                                                       ON peers.nickname = checks.peer
                                                  JOIN p2p
                                                       ON checks.id = p2p.check
                                         WHERE EXTRACT(DAY FROM peers.birthday) = EXTRACT(DAY FROM checks.date)
                                           AND EXTRACT(MONTH FROM peers.birthday) = EXTRACT(MONTH FROM checks.date)
                                           AND state = 'Failure') * 100 / total, 2);
BEGIN
    IF total > 0.0 THEN
        OPEN curs FOR SELECT SuccessfulChecks, UnsuccessfulChecks;
    END IF;
END;
$$ LANGUAGE plpgsql;

-- call ex13
BEGIN;
CALL birthday_checks();
FETCH ALL IN "result";
END;

-- ex14
-- Определить кол-во XP, полученное в сумме каждым пиром
CREATE OR REPLACE PROCEDURE sum_xp_peers(INOUT curs refcursor ='result') AS
$$
BEGIN
    OPEN curs FOR SELECT peer, SUM(max_xp) AS XP
                  FROM (SELECT "check", MAX(xp_amount) AS max_xp
                        FROM xp
                        GROUP BY xp."check") AS t_max_xp
                           JOIN checks ON checks.id = t_max_xp."check"
                  GROUP BY peer
                  ORDER BY XP DESC;
END
$$ LANGUAGE plpgsql;

-- call ex14
BEGIN;
CALL sum_xp_peers();
FETCH ALL FROM "result";
END;

-- ex15
-- Определить всех пиров, которые сдали заданные задания 1 и 2, но не сдали задание 3
CREATE OR REPLACE PROCEDURE pr_peers_did_tasks(IN task1 varchar, IN task2 varchar, IN task3 varchar,
                                               INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR (SELECT peer
                   FROM v_success_check
                   WHERE task = task1
                   INTERSECT
                   SELECT peer
                   FROM v_success_check
                   WHERE task = task2)
                  EXCEPT
                  SELECT peer
                  FROM v_success_check
                  WHERE task = task3;
END
$$ LANGUAGE plpgsql;

-- call ex15
BEGIN;
CALL pr_peers_did_tasks('C5_s21_decimal', 'CPP3_s21_smartcalc_2_0', 'C4_s21_math');
FETCH ALL FROM "result";
COMMIT;

--ex16
-- Используя рекурсивное обобщенное табличное выражение, для каждой задачи вывести кол-во предшествующих ей задач
CREATE OR REPLACE PROCEDURE prev_tasks(INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR WITH RECURSIVE previous_task(title, parenttask, count) AS
                                     (SELECT title, parent_task, 0
                                      FROM tasks
                                      WHERE parent_task IS NULL
                                      UNION ALL
                                      SELECT t.title, t.parent_task, count + 1
                                      FROM previous_task pt,
                                           tasks t
                                      WHERE pt.title = t.parent_task)
                  SELECT title AS task, count AS prevcount
                  FROM previous_task;
END;
$$ LANGUAGE plpgsql;

-- call ex16
BEGIN;
CALL prev_tasks();
FETCH ALL IN "result";
END;

-- ex17
-- Найти "удачные" для проверок дни. День считается "удачным", если в нем есть хотя бы N идущих подряд успешных проверки
CREATE OR REPLACE PROCEDURE pr_lucky_days(IN count_success int, INOUT curs refcursor='result') AS
$$
BEGIN
    OPEN curs FOR (SELECT "date"
                   FROM (SELECT "date",
                                (ROW_NUMBER() OVER (PARTITION BY "date", x_count_succ ORDER BY "date","time") -
                                 (CASE WHEN x_count_succ = 0 THEN 0 ELSE 1 END)) lucky_days
                         FROM (SELECT all_checks."date" AS                              "date",
                                      "time",
                                      bool_state        AS                              state,
                                      xp_amount,
                                      max_xp,
                                      COUNT(CASE WHEN bool_state = FALSE THEN 1 END)
                                      OVER (PARTITION BY "date" ORDER BY "date","time") x_count_succ
                               FROM (SELECT v_success_check.id, "date", task, TRUE AS bool_state
                                     FROM v_success_check
                                     UNION
                                     SELECT v_not_success_check.id, "date", task, FALSE AS bool_state
                                     FROM v_not_success_check) AS all_checks
                                        JOIN p2p ON p2p."check" = all_checks.id AND state = 'Start'
                                        JOIN tasks ON tasks.title = all_checks.task
                                        LEFT JOIN xp ON all_checks.id = xp."check"
                               ORDER BY all_checks."date", "time") sub_query
                         WHERE state = TRUE
                           AND xp_amount * 100.0 / max_xp >= 80) AS zcxc
                   GROUP BY "date"
                   HAVING MAX(lucky_days) >= count_success);
END;
$$ LANGUAGE plpgsql;

-- call ex17
BEGIN;
CALL pr_lucky_days(2);
FETCH ALL FROM "result";
COMMIT;

-- ex18
-- Определить пира с наибольшим числом выполненных заданий
CREATE OR REPLACE PROCEDURE pr_number_completed_tasks(INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR WITH cte AS (SELECT peer, COUNT(peer) AS xp FROM v_success_check GROUP BY peer)
                  SELECT peer, xp
                  FROM cte
                  WHERE xp = (SELECT MAX(xp) FROM cte);
END
$$ LANGUAGE plpgsql;

-- call ex18
BEGIN;
CALL pr_number_completed_tasks();
FETCH ALL FROM "result";
COMMIT;

--ex19
-- Определить пира с наибольшим количеством XP
CREATE PROCEDURE peer_max_xp(INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR SELECT peer, SUM(xp_amount) AS xp
                  FROM checks
                           JOIN xp
                                ON checks.id = xp.check
                  GROUP BY peer
                  ORDER BY xp DESC
                  LIMIT 1;
END;
$$ LANGUAGE plpgsql;

-- call ex19
BEGIN;
CALL peer_max_xp();
FETCH ALL IN "result";
END;

-- ex20
-- Определить пира, который провел сегодня в кампусе больше всего времени
CREATE OR REPLACE PROCEDURE pr_max_time_tracking_peer_today(INOUT curs refcursor='result') AS
$$
BEGIN
    OPEN curs FOR SELECT peer
                  FROM (WITH all_peers AS (SELECT * FROM time_tracking WHERE "date" = CURRENT_DATE),
                             enter_peer AS (SELECT peer, SUM("time") AS time_enter
                                            FROM all_peers
                                            WHERE state = 1
                                            GROUP BY peer),
                             exit_peer AS (SELECT peer, SUM("time") AS time_exit
                                           FROM all_peers
                                           WHERE state = 2
                                           GROUP BY peer)
                        SELECT enter_peer.peer, (time_exit - time_enter) AS time_campus
                        FROM enter_peer
                                 JOIN exit_peer
                                      ON enter_peer.peer = exit_peer.peer
                        ORDER BY time_campus DESC
                        LIMIT 1) peer_time;
END;
$$ LANGUAGE plpgsql;

-- call ex20
BEGIN;
CALL pr_max_time_tracking_peer_today();
FETCH ALL FROM "result";
COMMIT;

-- ex21
-- Определить пиров, приходивших раньше заданного времени не менее N раз за всё время
CREATE OR REPLACE PROCEDURE pr_peers_came_before_time(IN t time, IN n integer, INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR WITH cte AS (SELECT peer, COUNT(peer) AS count
                               FROM time_tracking
                               WHERE state = 1
                                 AND "time" < t
                               GROUP BY peer)
                  SELECT peer
                  FROM cte
                  WHERE count >= n;
END
$$ LANGUAGE plpgsql;

-- call ex21
BEGIN;
CALL pr_peers_came_before_time('18:00:00', 2);
FETCH ALL FROM "result";
COMMIT;

--ex22
-- Определить пиров, выходивших за последние N дней из кампуса больше M раз
CREATE OR REPLACE PROCEDURE out_of_campus(n integer, m integer, INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR SELECT peer
                  FROM (SELECT peer, date, COUNT(*) AS c
                        FROM time_tracking
                        WHERE state = 2
                          AND date >= (NOW()::date - n)
                        GROUP BY peer, date) AS a
                  GROUP BY peer
                  HAVING SUM(c) > m
                  ORDER BY peer;
END
$$ LANGUAGE plpgsql;

-- call ex22
BEGIN;
CALL out_of_campus(365, 0);
FETCH ALL FROM "result";
END;

-- ex23
-- Определить пира, который пришел сегодня последним
CREATE OR REPLACE PROCEDURE pr_last_peer_today(INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR SELECT peer
                  FROM (SELECT DISTINCT ON (peer) peer, time
                        FROM time_tracking
                        WHERE "date" = CURRENT_DATE
                          AND state = 1
                        ORDER BY peer, "time") AS peer_time
                  ORDER BY time DESC
                  LIMIT 1;
END
$$ LANGUAGE plpgsql;

-- call ex23
BEGIN;
CALL pr_last_peer_today();
FETCH ALL FROM "result";
COMMIT;

-- ex24
-- Определить пиров, которые выходили вчера из кампуса больше чем на N минут
CREATE OR REPLACE PROCEDURE pr_left_campus_yesterday(IN minutes integer, INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR WITH enter AS (SELECT * FROM time_tracking WHERE state = 1 AND "date" = CURRENT_DATE - 1),
                       exit AS (SELECT * FROM time_tracking WHERE state = 2 AND "date" = CURRENT_DATE - 1),
                       left_campus AS (SELECT DISTINCT ON (exit.id) exit.id,
                                                                    exit.peer,
                                                                    exit.time  AS exit,
                                                                    enter.time AS enter
                                       FROM exit
                                                JOIN enter ON enter.peer = exit.peer
                                       WHERE enter.time > exit.time)
                  SELECT peer
                  FROM left_campus
                  WHERE (enter - exit) > MAKE_TIME(minutes / 60, minutes - minutes / 60 * 60, 0.0)
                  GROUP BY peer;
END
$$ LANGUAGE plpgsql;

-- call ex24
BEGIN;
CALL pr_left_campus_yesterday(10);
FETCH ALL FROM "result";
COMMIT;

--ex25
-- Определить для каждого месяца процент ранних входов
CREATE OR REPLACE PROCEDURE months_stat(INOUT curs refcursor = 'result') AS
$$
BEGIN
    OPEN curs FOR WITH months AS
                           (SELECT ROW_NUMBER() OVER () AS num, TO_CHAR(gs, 'Month') AS month
                            FROM (SELECT generate_series AS gs
                                  FROM GENERATE_SERIES('2023-01-01', '2023-12-31', INTERVAL '1 month')) AS s)
                  SELECT month,
                         COALESCE((SELECT COUNT(*) * 100 / NULLIF((SELECT COUNT(*)
                                                                   FROM peers p1
                                                                            JOIN time_tracking t1
                                                                                 ON p1.nickname = t1.peer
                                                                   WHERE EXTRACT(MONTH FROM p1.birthday) = EXTRACT(MONTH FROM t1.date)
                                                                     AND t1.state = 1
                                                                     AND num = EXTRACT(MONTH FROM t1.date)), 0)
                                   FROM peers p
                                            JOIN time_tracking t2
                                                 ON p.nickname = t2.peer
                                   WHERE EXTRACT(MONTH FROM p.birthday) = EXTRACT(MONTH FROM t2.date)
                                     AND num = EXTRACT(MONTH FROM t2.date)
                                     AND t2.state = 1
                                     AND EXTRACT(HOURS FROM t2.time) < 12), 0) AS earlyentries
                  FROM months;
END;
$$ LANGUAGE plpgsql;

-- call ex25
BEGIN;
CALL months_stat();
FETCH ALL IN "result";
END;