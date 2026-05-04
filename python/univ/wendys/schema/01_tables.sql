CREATE TABLE IF NOT EXISTS cd (
    cd_number INT          AUTO_INCREMENT PRIMARY KEY,
    title     VARCHAR(255) NOT NULL CHECK (TRIM(title) <> ''),
    producer  VARCHAR(255) NOT NULL CHECK (TRIM(producer) <> ''),
    year      SMALLINT     NOT NULL CHECK (year >= 1982),
    copies    INT          NOT NULL CHECK (copies >= 0)
);

CREATE TABLE IF NOT EXISTS client (
    client_number INT          AUTO_INCREMENT PRIMARY KEY,
    first_name    VARCHAR(255) NOT NULL CHECK (TRIM(first_name) <> ''),
    last_name     VARCHAR(255) NOT NULL CHECK (TRIM(last_name) <> ''),
    email_address VARCHAR(255) NOT NULL UNIQUE CHECK (email_address LIKE '%@%._%'),
    phone_number  VARCHAR(20)
);

CREATE TABLE IF NOT EXISTS contains (
    playlist     VARCHAR(255) NOT NULL,
    track_number INT          NOT NULL,
    cd_number    INT          NOT NULL,

    PRIMARY KEY (playlist, track_number, cd_number)
);

CREATE TABLE IF NOT EXISTS dj (
    id INT PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS employee (
    id        INT          AUTO_INCREMENT PRIMARY KEY,
    firstname VARCHAR(255) NOT NULL CHECK (TRIM(firstname) <> ''),
    lastname  VARCHAR(255) NOT NULL CHECK (TRIM(lastname) <> '')
);

CREATE TABLE IF NOT EXISTS event (
    id            INT           AUTO_INCREMENT PRIMARY KEY,
    name          VARCHAR(255)  NOT NULL CHECK (TRIM(name) <> ''),
    date          DATE          NOT NULL,
    description   TEXT                   CHECK (TRIM(description) <> ''),
    client        INT           NOT NULL,
    manager       INT,
    event_planner INT,
    dj            INT,
    theme         VARCHAR(255)  NOT NULL,
    type          VARCHAR(255)  NOT NULL CHECK (TRIM(type) <> ''),
    location      INT,
    rental_fee    DECIMAL(10,2) CHECK (rental_fee >= 0),
    playlist      VARCHAR(255)
);

CREATE TABLE IF NOT EXISTS eventplanner (
    id INT PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS genre (
    name VARCHAR(255) PRIMARY KEY CHECK (TRIM(name) <> '')
);

CREATE TABLE IF NOT EXISTS location (
    id          INT          AUTO_INCREMENT PRIMARY KEY,
    street      VARCHAR(255) NOT NULL CHECK (TRIM(street) <> ''),
    city        VARCHAR(255) NOT NULL CHECK (TRIM(city) <> ''),
    postal_code VARCHAR(255),
    country     VARCHAR(255) NOT NULL CHECK (TRIM(country) <> ''),
    comment     VARCHAR(500)
);

CREATE TABLE IF NOT EXISTS manager (
    id INT PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS playlist (
    name VARCHAR(255) PRIMARY KEY CHECK (TRIM(name) <> '')
);

CREATE TABLE IF NOT EXISTS request (
    event_id INT           NOT NULL,
    name     VARCHAR(255)  NOT NULL CHECK (TRIM(name) <> ''),
    provider VARCHAR(255)  NOT NULL CHECK (TRIM(provider) <> ''),
    price    DECIMAL(10,2) NOT NULL CHECK (price >= 0),

    PRIMARY KEY (event_id, name)
);

CREATE TABLE IF NOT EXISTS song (
    cd_number    INT           NOT NULL,
    track_number INT           NOT NULL CHECK (track_number > 0),
    title        VARCHAR(255)  NOT NULL CHECK (TRIM(title) <> ''),
    artist       VARCHAR(255)  NOT NULL CHECK (TRIM(artist) <> ''),
    duration     TIME          NOT NULL CHECK (duration > 0),
    genre        VARCHAR(255)  NOT NULL,

    PRIMARY KEY (cd_number, track_number)
);

CREATE TABLE IF NOT EXISTS specialization (
    dj    INT          NOT NULL,
    genre VARCHAR(255) NOT NULL,

    PRIMARY KEY (dj, genre)
);

CREATE TABLE IF NOT EXISTS specializes (
    subgenre VARCHAR(255) NOT NULL,
    genre    VARCHAR(255) NOT NULL,

    PRIMARY KEY (subgenre, genre)
);

CREATE TABLE IF NOT EXISTS suitablefor (
    theme    VARCHAR(255) NOT NULL,
    playlist VARCHAR(255) NOT NULL,

    PRIMARY KEY (theme, playlist)
);

CREATE TABLE IF NOT EXISTS supervision (
    employee_id   INT PRIMARY KEY,
    supervisor_id INT
);

CREATE TABLE IF NOT EXISTS theme (
    name VARCHAR(255) PRIMARY KEY CHECK (TRIM(name) <> '')
);
