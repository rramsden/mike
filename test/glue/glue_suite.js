try {
    test = require('test');
    assert = require('assert');

    var suite = {};
    require('test_file');
    result = test.run(suite);

    puts("\n\nOK (" + test.passed + " suites)");

    if (result > 0) {
	var i, len;
	var failures = test.failures;

	puts("FAILED (" + result + " suites)\n");
	
	for (i = 0; i < result; i++) {
	    var failure = failures[i]; 
	    puts((i+1) + ".) "+ failure.name);
	    puts("    " + failure.error.toString());
	}
    }

    puts("\n");
    result;
} catch (err) {
    Stderr.write(err+"\n");
    throw(err);
}