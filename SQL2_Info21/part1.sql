DROP TYPE IF EXISTS check_status CASCADE;
CREATE TYPE check_status AS ENUM ('Start', 'Success', 'Failure');

DROP TABLE IF EXISTS Peers CASCADE;
CREATE TABLE IF NOT EXISTS Peers
(
    nickname varchar PRIMARY KEY,
    birthday date
);

DROP TABLE IF EXISTS Tasks CASCADE;
CREATE TABLE IF NOT EXISTS Tasks
(
    title       varchar PRIMARY KEY,
    parent_task varchar DEFAULT NULL REFERENCES Tasks (title),
    max_xp      integer
);

DROP TABLE IF EXISTS Checks CASCADE;
CREATE TABLE IF NOT EXISTS Checks
(
    id     bigint PRIMARY KEY,
    peer   varchar,
    task   varchar,
    "date" date,
    CONSTRAINT fk_checks_peer FOREIGN KEY (peer) REFERENCES Peers (nickname),
    CONSTRAINT fk_checks_task FOREIGN KEY (task) REFERENCES Tasks (title)
);

DROP TABLE IF EXISTS P2P CASCADE;
CREATE TABLE IF NOT EXISTS P2P
(
    id            bigint PRIMARY KEY,
    "check"       bigint,
    checking_peer varchar,
    state         check_status DEFAULT 'Start',
    "time"        time NOT NULL,
    CONSTRAINT fk_p2p_check FOREIGN KEY ("check") REFERENCES Checks (id),
    CONSTRAINT fk_p2p_checking_peer FOREIGN KEY (checking_peer) REFERENCES Peers (nickname)
);

CREATE OR REPLACE FUNCTION fnc_trg_p2p_check_incomplete_status() RETURNS trigger
AS
$$
DECLARE
    start_count   bigint;
    success_count bigint;
    failure_count bigint;
    last_time     time := (SELECT time
                           FROM P2P
                           WHERE "check" = NEW."check"
                             AND checking_peer = NEW.checking_peer
                           ORDER BY time DESC
                           LIMIT 1);
BEGIN
    WITH cte_count_rows AS (SELECT state
                            FROM P2P
                            WHERE "check" = NEW."check"
                              AND checking_peer = NEW.checking_peer),
         cte_start_count AS (SELECT COUNT(*)
                             FROM cte_count_rows
                             WHERE state = 'Start'),
         cte_success_count AS (SELECT COUNT(*)
                               FROM cte_count_rows
                               WHERE state = 'Success'),
         cte_failure_count AS (SELECT COUNT(*)
                               FROM cte_count_rows
                               WHERE state = 'Failure')
    SELECT *
    FROM cte_start_count,
         cte_success_count,
         cte_failure_count
    INTO start_count , success_count , failure_count;

    IF (TG_OP = 'INSERT') THEN
        IF (last_time IS NOT NULL AND (NEW.time <= last_time)) OR
           ((NEW.state = 'Start') AND (start_count > success_count + failure_count))
            OR ((NEW.state = 'Success' OR NEW.state = 'Failure') AND
                (start_count <> success_count + failure_count + 1))
        THEN
            RETURN NULL;
        END IF;
        RETURN NEW;
    END IF;
END ;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER trg_p2p_check_incomplete_status
    BEFORE INSERT
    ON P2P
    FOR EACH ROW
EXECUTE FUNCTION fnc_trg_p2p_check_incomplete_status();

DROP TABLE IF EXISTS Verter CASCADE;
CREATE TABLE IF NOT EXISTS Verter
(
    id      bigint PRIMARY KEY,
    "check" bigint,
    state   check_status DEFAULT 'Start',
    "time"  time,
    CONSTRAINT fk_verter_check FOREIGN KEY ("check") REFERENCES Checks (id)
);

DROP TABLE IF EXISTS Transferred_Points CASCADE;
CREATE TABLE IF NOT EXISTS Transferred_Points
(
    id            bigint PRIMARY KEY,
    checking_peer varchar,
    checked_peer  varchar,
    points_amount integer,
    CONSTRAINT fk_transferred_points_checking_peer FOREIGN KEY (checking_peer) REFERENCES Peers (nickname),
    CONSTRAINT fk_transferred_points_checked_peer FOREIGN KEY (checked_peer) REFERENCES Peers (nickname)
);

DROP TABLE IF EXISTS Friends CASCADE;
CREATE TABLE IF NOT EXISTS Friends
(
    id    bigint PRIMARY KEY,
    peer1 varchar,
    peer2 varchar,
    CONSTRAINT fk_friends_peer1 FOREIGN KEY (peer1) REFERENCES Peers (nickname),
    CONSTRAINT fk_friends_peer2 FOREIGN KEY (peer2) REFERENCES Peers (nickname)
);

DROP TABLE IF EXISTS Recommendations CASCADE;
CREATE TABLE IF NOT EXISTS Recommendations
(
    id               bigint PRIMARY KEY,
    peer             varchar,
    recommended_peer varchar,
    CONSTRAINT fk_recommendations_peer FOREIGN KEY (peer) REFERENCES Peers (nickname),
    CONSTRAINT fk_recommendations_recommended_peer FOREIGN KEY (recommended_peer) REFERENCES Peers (nickname)
);

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

CREATE OR REPLACE VIEW v_not_success_check AS
SELECT checks.id, peer, task, date
FROM checks
WHERE checks.id IN (SELECT "check"
                    FROM p2p
                    WHERE state = 'Failure'
                    UNION
                    SELECT "check"
                    FROM verter
                    WHERE state = 'Failure');

DROP TABLE IF EXISTS XP;
CREATE TABLE IF NOT EXISTS XP
(
    id        bigint PRIMARY KEY,
    "check"   bigint,
    xp_amount integer,
    CONSTRAINT fk_xp_check FOREIGN KEY ("check") REFERENCES Checks (id),
    CONSTRAINT fk_xp_xp_amount CHECK ( xp_amount > 0 ),
    CONSTRAINT fk_xp_check_unique UNIQUE ("check")
);

DROP TABLE IF EXISTS Time_Tracking CASCADE;
CREATE TABLE IF NOT EXISTS Time_Tracking
(
    id     bigint PRIMARY KEY,
    peer   varchar,
    "date" date,
    "time" time,
    state  integer NOT NULL DEFAULT 1,
    CONSTRAINT fk_time_tracking_peer FOREIGN KEY (peer) REFERENCES Peers (nickname),
    CONSTRAINT ch_state CHECK (state IN (1, 2))
);

-- !!! Please check absolute path of the project
SET path_to_src.var TO '/Users/rdexter/me/school21/SQL2_Info21_v1.0-0/src/data/';

-- import data to tables from csv files
CREATE OR REPLACE PROCEDURE pr_import_from_csv(IN tablename text, IN path text, IN delim text) AS
$$
BEGIN
    EXECUTE FORMAT('COPY %s FROM %L WITH DELIMITER ''%s'' CSV HEADER', $1, $2, $3);
END;
$$ LANGUAGE plpgsql;

CALL pr_import_from_csv('Peers', CURRENT_SETTING('path_to_src.var') || 'peers.csv', ',');
CALL pr_import_from_csv('Tasks', CURRENT_SETTING('path_to_src.var') || 'tasks.csv', ',');
CALL pr_import_from_csv('Checks', CURRENT_SETTING('path_to_src.var') || 'checks.csv', ',');
CALL pr_import_from_csv('P2P', CURRENT_SETTING('path_to_src.var') || 'p2p.csv', ',');
CALL pr_import_from_csv('Verter', CURRENT_SETTING('path_to_src.var') || 'verter.csv', ',');
CALL pr_import_from_csv('XP', CURRENT_SETTING('path_to_src.var') || 'xp.csv', ',');
CALL pr_import_from_csv('Friends', CURRENT_SETTING('path_to_src.var') || 'friends.csv', ',');
CALL pr_import_from_csv('Recommendations', CURRENT_SETTING('path_to_src.var') || 'recommendations.csv', ',');
CALL pr_import_from_csv('Time_Tracking', CURRENT_SETTING('path_to_src.var') || 'time_tracking.csv', ',');
CALL pr_import_from_csv('Transferred_Points', CURRENT_SETTING('path_to_src.var') || 'transferred_points.csv',
                        ',');

-- export data from tables to csv files
CREATE OR REPLACE PROCEDURE pr_export_to_csv(IN tablename text, IN path text, IN delim text) AS
$$
BEGIN
    EXECUTE FORMAT('COPY %s TO %L DELIMITER ''%s'' CSV HEADER;', $1, $2, $3);
END;
$$ LANGUAGE plpgsql;

CALL pr_export_to_csv('Transferred_Points', CURRENT_SETTING('path_to_src.var') || 'transferred_points.csv',
                      ',');
CALL pr_export_to_csv('P2P', CURRENT_SETTING('path_to_src.var') || 'p2p.csv', ',');
CALL pr_export_to_csv('Checks', CURRENT_SETTING('path_to_src.var') || 'checks.csv', ',');
CALL pr_export_to_csv('Verter', CURRENT_SETTING('path_to_src.var') || 'verter.csv', ',');
CALL pr_export_to_csv('XP', CURRENT_SETTING('path_to_src.var') || 'xp.csv', ',');
CALL pr_export_to_csv('Friends', CURRENT_SETTING('path_to_src.var') || 'friends.csv', ',');
CALL pr_export_to_csv('Peers', CURRENT_SETTING('path_to_src.var') || 'peers.csv', ',');
CALL pr_export_to_csv('Recommendations', CURRENT_SETTING('path_to_src.var') || 'recommendations.csv', ',');
CALL pr_export_to_csv('Tasks', CURRENT_SETTING('path_to_src.var') || 'tasks.csv', ',');
CALL pr_export_to_csv('Time_Tracking', CURRENT_SETTING('path_to_src.var') || 'time_tracking.csv', ',');



