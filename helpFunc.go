package exprtk

import (
	"strings"
)

var ReservedList = []string{"if", "else", "for", "break", "and", "true", "abs", "var",
	"max", "min", "sum", "acos", "acosh", "asin", "asinh", "atan",
	"atanh", "atan2", "avg", "case", "ceil", "clamp", "mod",
	"continue", "cos", "cosh", "cot", "csc", "default",
	"deg2grad", "deg2rad", "equal", "erf", "erfc", "exp",
	"expm1", "false", "floor", "frac", "grad2deg", "nand",
	"hypot", "iclamp", "ilike", "in", "inrange", "mor", "nor",
	"like", "log10", "log1p", "mand", "mul", "ncdf", "log2",
	"not", "not_equal", "null", "or", "pow", "rad2deg", "logn",
	"repeat", "return", "root", "round", "roundn", "sec", "sgn",
	"shl", "shr", "sin", "sinc", "sinh", "sqrt", "swap",
	"switch", "tan", "tanh", "trunc", "until", "log",
	"while", "xnor", "xor", "&", "|",
	"x_sum", "x_avg", "x_min", "x_max", "x_first", "x_last", "x_dev", "x_diff", "x_wavg"}

// c是字母
func isLetter(c byte) bool {
	return (('a' <= c) && (c <= 'z')) || (('A' <= c) && (c <= 'Z'))
}

// c是数字
func isDigit(c byte) bool {
	return ('0' <= c) && (c <= '9')
}

// c是字母或数字
func IsLetterOrDigit(c byte) bool {
	return isLetter(c) || isDigit(c)
}

// 是数字字符串
func isDigitString(str string) bool {
	for i := 0; i < len(str); i++ {
		if !isDigit(str[i]) {
			return false
		}
	}
	return true
}

func isWhiteSpace(c byte) bool {
	return (c == ' ') || (c == '\n') ||
		(c == '\r') || (c == '\t') ||
		(c == '\b') || (c == '\v') ||
		(c == '\f')
}

// 是操作符
func isOperatorChar(c byte) bool {
	return (c == '+') || (c == '-') ||
		(c == '*') || (c == '/') ||
		(c == '^') || (c == '<') ||
		(c == '>') || (c == '=') ||
		(c == ',') || (c == '!') ||
		(c == '(') || (c == ')') ||
		(c == '[') || (c == ']') ||
		(c == '{') || (c == '}') ||
		(c == '%') || (c == ':') ||
		(c == '?') || (c == '&') ||
		(c == '|') || (c == ';')
}

// 是保留字符串
func isReservedSymbols(str string) bool {
	str = strings.ToLower(str)
	for _, v := range ReservedList {
		if strings.Compare(v, str) == 0 {
			return true
		}
	}

	return false
}

// 是常量字符串
func isConstStr(str string) bool {
	if strings.HasPrefix(str, "'") && strings.HasSuffix(str, "'") {
		return true
	}
	return false
}

// 表达式字符串格式化 特殊符号转换
func expressionFormat(str string) string {
	//str = strings.ReplaceAll(str,":","__")
	str = strings.ReplaceAll(str, "**", "likelike")
	str = strings.ReplaceAll(str, "?", "anyany")
	return str
}

// 表达式字符串恢复 特殊符号还原
func expressionRestore(str string) string {
	str = strings.ReplaceAll(str, "likelike", "**")
	str = strings.ReplaceAll(str, "anyany", "?")
	return str
}

// 表达式校验
func ExpressionVerify(expr string) error {
	//表达式计算 实例对象
	obj := NewExprtk()
	defer obj.Free()

	//设置表达式
	obj.SetExpression(expr)
	//获取变量列表
	vs := obj.GetVariableList()
	for _, v := range vs {
		obj.AddDoubleVariable(v, 0)
	}
	//表达式编译
	return obj.CompileExpression()
}
