import * as express from 'express';
import * as cookieParser from 'cookie-parser';
import 'dotenv/config';
import * as cors from 'cors';
import * as morgan from 'morgan';
import { exit } from 'process';
import { createTerminus } from '@godaddy/terminus';
import classify from './routes/classify';
import { logger, httpLogStream } from './logger/logger';

export const app = express();
const run = async () => {
    try {
        const port = process.env.GUI_PORT ? process.env.GUI_PORT : 8000;
        app.use(express.static('public'));
        app.use(
            cors({
                origin: true,
                credentials: true,
                optionsSuccessStatus: 200,
            }),
        );
        app.use(
            express.json({
                limit: '50mb',
            }),
        );
        app.use(
            express.urlencoded({
                extended: false,
                limit: '50mb',
            }),
        );

        app.use(cookieParser());
        app.use(
            morgan(`":method :url HTTP/:http-version" :status :res[content-length] ":referrer" ":user-agent"`, {
                stream: httpLogStream,
            }),
        );

        app.use((req, res, next) => {
            res.header('Content-Type', 'application/json;charset=UTF-8');
            res.header('Access-Control-Allow-Credentials', 'true');
            res.header('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept');
            next();
        });
        /**routes */
        app.use('/', classify);

        const onSignal = async () => {};

        const onHealthCheck = async () => {
            return true;
        };

        const server = app.listen(+port, () => {
            logger.info(`server has started ${port}`);
        });

        createTerminus(server, {
            signal: 'SIGINT',
            healthChecks: {
                '/_health/liveness': onHealthCheck,
            },
            onSignal,
        });
    } catch (ex) {
        logger.error(`run error : ${ex.message}`);
        exit(1);
    }
};

run();
