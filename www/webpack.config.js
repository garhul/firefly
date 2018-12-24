'use strict';

const webpack = require('webpack');
const copyWebpackPlugin = require('copy-webpack-plugin');
// const uglifyJs = require('uglifyjs-webpack-plugin');
const babel = require("babel-webpack-plugin");

const path = require('path');
const INPUT = path.join(__dirname,'dev');
const OUTPUT= path.join(__dirname,'dist');


function samplePlugin(cfg){}

samplePlugin.prototype.apply = function (compiler) {
  // var self = this;
  // var folder = compiler.options.context;
  // var entry = path.join(folder, self.entry);
  // var output = path.join(folder, self.output);
  console.log("entrada");
  console.log(compiler)
}
  // fs.readFile(entry, 'utf8', function (err, data) {
  //   if (!self.skip) {
  //     var matches = regexMatchAll(data, regex);
  //     matches.forEach(function (match) {
  //       var str = match[0];
  //       var key = match[4];
  //       data = data.replace(str, '\n' + self.data[key] + '\n');
  //     });
  //   }
  //
  //   compiler.plugin('done', function (stats) {
  //     if (self.hash) {
  //       var reg = new RegExp('\\' + self.hash, 'g');
  //       var changeWith = typeof self.hashValue === 'string' ? self.hashValue : stats.hash;
  //       data = data.replace(reg, changeWith);
  //     }
  //     fs.writeFileSync(output, data);
  //   });
  // });


var entries = {
	tv:path.join(INPUT,'tv','js', 'app.js'),
	setup:path.join(INPUT,'setup', 'js' ,'app.js')
};

module.exports = {
	entry:entries,
	output: {
		path: OUTPUT,
		filename: "[name].min.js"
	},
	devtool: 'inline-source-map',
	devServer: {
		contentBase: INPUT,
		compress: false,
		port: 9000,
		inline:true
	},
	plugins: [
		new copyWebpackPlugin([
			{
				from:path.join(INPUT, '**/index.html'),
				to:path.join(OUTPUT,'[name].index.html')
			}
		]),
    // new babel({
    //      test: /\.js$/,
    //      sourceMaps: false,
    //      presets: ['es2015'],
    //      compact: false
    // }),
		new samplePlugin({})
	]
}
