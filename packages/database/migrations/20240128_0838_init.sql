CREATE TABLE measurements (
    id SERIAL PRIMARY KEY,
    temperature FLOAT NOT NULL,
    humidity FLOAT NOT NULL,
    co2 INT2 NOT NULL,
    pm2p5 INT2 NOT NULL,
    uptime INT4 NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT NOW()
)
