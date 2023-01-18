/// Accepts a vector of T and creates a String
#[macro_export]
macro_rules! bulk_print {
    ($vec:expr, $s:expr) => {
        $vec.iter()
            .map(|x| x.print())
            .collect::<Vec<String>>()
            .join($s)
    };
}

#[macro_export]
macro_rules! parse {
    ($source:expr, $expected:expr) => {
        let source = String::from($source);

        let mut lexer = Lexer::new(&source);
        lexer.tokenize();
        lexer.report_errors("<input>");

        let mut parser = Parser::new(&lexer.tokens);
        parser.parse(&lexer.tokens);
        parser.report_errors("<input>", &source);

        let result = Expr::pretty_print(&parser.exprs);
        assert_eq!(result, $expected);
    };
}
