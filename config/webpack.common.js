module.exports = {
    entry: './src/index.ts',
    target: 'node',
    output: {
        filename: 'bundle.js', // output file
        path: `${__dirname}/../build`,
        libraryTarget: 'commonjs',
        devtoolModuleFilenameTemplate: '[absolute-resource-path]',
    },
    resolve: {
        extensions: ['.webpack.js', '.web.js', '.ts', '.tsx', '.js'],
        //modules: ['node_modules'],
    },
    module: {
        rules: [
            {
                // all files with a `.ts` or `.tsx` extension will be handled by `ts-loader`
                test: /\.tsx?$/,
                use: [
                    {
                        loader: 'ts-loader',
                    },
                ],
            },
            {
                test: /\.mjs$/,
                include: /node_modules/,
                type: 'javascript/auto',
            },
        ],
    },
    // stats: { warnings: false },
    externals: { express: 'commonjs express' },
};
