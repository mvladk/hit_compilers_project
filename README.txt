/**
 * c/c++
 *
 * Art Brindatch(tm) : mvcompiler (http://art-brindatch.com)
 * Copyright 2005-2013, Art Brindatch(Vladimir Karpov), Ltd. (http://art-brindatch.com)
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @copyright     Copyright 2005-2013, Art Brindatch(Vladimir Karpov), Ltd. (http://art-brindatch.com)
 * @link          http://art-brindatch.com mvcompiler(tm) Project
 * @since         mvcompiler (tm) v 1.0.0
 * @license       MIT License (http://www.opensource.org/licenses/mit-license.php)
 */
 
Build and usage instructions
How to, step by step:

1. Generate lex.yy.c by running flex in command line:
hit_compilers_project\source files>flex exprs.lex

2. Build the solution located in hit_compilers_project\vs2010\mvcompiler using Visual Studio 2010 

3. Compile your language in command line, for example:
hit_compilers_project\vs2010\mvcompiler\Debug\mvcompiler.exe < "_FULLPATH_\hit_compilers_project\Results of execution\input_correct_2.tx
t"