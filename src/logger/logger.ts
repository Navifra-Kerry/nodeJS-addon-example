import { createLogger, format, transports, transport } from 'winston';
import * as DailyRotateFile from 'winston-daily-rotate-file';

const env = process.env.NODE_ENV || 'development';
const logDir = 'log';

const dailyRotateFileTransport = new DailyRotateFile({
    level: 'debug',
    filename: `${logDir}/%DATE%-server.log`,
    datePattern: 'YYYY-MM-DD',
    zippedArchive: true,
    maxSize: '20m',
    maxFiles: '14d',
});

export const logger = createLogger({
    exitOnError: false,
    level: env === 'development' ? 'debug' : 'info',
    format: format.combine(
        format.timestamp({
            format: 'DD/MMM/YYYY:HH:mm:ss ZD',
        }),
        format((info) => {
            info.level = info.level.toUpperCase();
            return info;
        })(),
        format.json(),
    ),
    exceptionHandlers: [new transports.File({ filename: `${logDir}/exceptions.log` })],
    transports: [
        new transports.Console({
            level: 'http',
            format: format.combine(
                format.colorize(),
                format.printf((info) => `[${info.timestamp}] [${info.level}] ${info.message}`),
            ),
            silent: false,
        }),
        dailyRotateFileTransport,
    ],
});

export const httpLogStream = {
    write: (message: string) => {
        logger.http(message);
    },
};

logger.on('error', (err) => {});
