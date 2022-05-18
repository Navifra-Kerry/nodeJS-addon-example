const merge = require('webpack-merge').default;
const common = require('./webpack.common.js');

module.exports = merge(common, {
    mode: 'production',
    devtool: 'cheap-module-source-map',
    module: {
        rules: [
            {
                test: /\.node$/,
                loader: 'native-ext-loader',
                options: {
                    rewritePath: `${__dirname}/../build`,
                },
            },
            {
                test: /\.js$/,
                loader: 'node-bindings-loader',
            },
        ],
    },
});
