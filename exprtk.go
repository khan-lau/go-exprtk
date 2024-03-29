package exprtk

// import "C"

// #cgo CXXFLAGS: -std=c++11 -Ofast -flto
// #cgo LDFLAGS: -L.
// #include "exprtkwrapper.h"
// #include <stdlib.h>
// #include <stdio.h>
import "C"

import (
	"errors"
	"strings"
	"unsafe"
)

// GoExprtk ...Exprtk Structure
type GoExprtk struct {
	exprCache    string   //表达式缓存
	varListCache []string //变量列表缓存
	funNameCache []string //用户函数名缓存
	exprtk       C.exprtkWrapper
}

// NewExprtk ... Creates a new object 调用完后请使用FreeExprtk、Free释放内存
func NewExprtk() GoExprtk {
	var obj GoExprtk
	obj.exprtk = C.exprtkWrapperInit()
	return obj
}

// 释放内存
func FreeExprtk(obj GoExprtk) {
	C.freeExprtk(obj.exprtk)
}

// 释放内存
func (obj *GoExprtk) Free() {
	C.freeExprtk(obj.exprtk)
}

// 设置表达式 请通过GetVariableList()获取自动注册的变量列表
func (obj *GoExprtk) SetExpression(expr string) {
	obj.exprCache = expr

	expr = expressionFormat(expr) //格式化
	str := C.CString(expr)
	defer C.free(unsafe.Pointer(str))

	C.setExpressionString(obj.exprtk, str)
	//obj.autoCheckVariable()
}

// AddDoubleVariable ... Adds variable to the expression
func (obj GoExprtk) AddDoubleVariable(x string, val float64) {
	x = expressionFormat(x) //格式化

	str := C.CString(x)
	defer C.free(unsafe.Pointer(str))

	C.addDoubleVariable(obj.exprtk, str, C.double(val))
}

// AddStringVariable ... Adds variable to the expression
func (obj GoExprtk) AddStringVariable(varName string, val string) {
	varName = expressionFormat(varName) //格式化

	sName := C.CString(varName)
	sVal := C.CString(val)
	defer C.free(unsafe.Pointer(sName))
	defer C.free(unsafe.Pointer(sVal))
	C.addStringVariable(obj.exprtk, sName, sVal)
}

// AddVectorVariable ... Adds variable to the expression
func (obj GoExprtk) AddVectorVariable(x string, val []float64) {
	x = expressionFormat(x) //格式化

	str := C.CString(x)
	defer C.free(unsafe.Pointer(str))

	arr := make([]C.double, 0, len(val)+1)
	for i := 0; i < len(val); i++ {
		arr = append(arr, C.double(val[i]))
	}

	if len(val) == 0 {
		arr = append(arr, C.double(0))
	}

	C.addVectorVariable(obj.exprtk, str, &arr[0], C.int(len(val)))
}

// SetDoubleVariableValue ... Sets value to the variable
func (obj GoExprtk) UpdateDoubleVariableValue(varName string, val float64) {
	varName = expressionFormat(varName) //格式化

	str := C.CString(varName)
	defer C.free(unsafe.Pointer(str))

	C.setDoubleVariableValue(obj.exprtk, str, C.double(val))
}

// SetStringVariableValue ... Sets value to the variable
func (obj GoExprtk) UpdateStringVariableValue(varName string, val string) {
	varName = expressionFormat(varName) //格式化

	sName := C.CString(varName)
	sVal := C.CString(val)
	defer C.free(unsafe.Pointer(sName))
	defer C.free(unsafe.Pointer(sVal))

	C.setStringVariableValue(obj.exprtk, sName, sVal)
}

// SetVectorVariableValue ... Sets value to the variable
func (obj GoExprtk) UpdateVectorVariableValue(varName string, val []float64) {
	varName = expressionFormat(varName) //格式化

	//arr := make([]C.double, 0)
	arr := make([]C.double, 0, len(val)+1)

	for i := 0; i < len(val); i++ {
		arr = append(arr, C.double(val[i]))
	}
	if len(val) == 0 {
		arr = append(arr, C.double(0))
	}
	firstValue := &(arr[0])
	var arrayLength C.int = C.int(len(arr))

	sName := C.CString(varName)
	defer C.free(unsafe.Pointer(sName))
	//defer C.free(unsafe.Pointer(firstValue))
	C.setVectorVariableValue(obj.exprtk, sName, firstValue, arrayLength)
}

// CompileExpression ... Compiles the Expression
func (obj *GoExprtk) CompileExpression() error {
	value := C.compileExpression(obj.exprtk)
	if value != 1 {
		return errors.New(obj.getCompileError())
	}
	return nil
}

func (obj GoExprtk) getCompileError() string {
	return C.GoString(C.printParseErr(obj.exprtk))
}

// GetEvaluatedValue ... Returns the evaluated value
func (obj GoExprtk) GetEvaluatedValue() float64 {
	return float64(C.getEvaluatedValue(obj.exprtk))
}

// 添加自定义函数
func (obj *GoExprtk) AddFunction(funName string, cFun unsafe.Pointer, argNum int) {
	sName := C.CString(funName)
	defer C.free(unsafe.Pointer(sName))
	switch argNum {
	case 0:
		C.addFunction(obj.exprtk, sName, C.fun_double(cFun))
	case 1:
		C.addFunction1(obj.exprtk, sName, C.fun1_double(cFun))
	case 2:
		C.addFunction2(obj.exprtk, sName, C.fun2_double(cFun))
	case 3:
		C.addFunction3(obj.exprtk, sName, C.fun3_double(cFun))
	case 4:
		C.addFunction4(obj.exprtk, sName, C.fun4_double(cFun))
	case 5:
		C.addFunction5(obj.exprtk, sName, C.fun5_double(cFun))
	case 6:
		C.addFunction6(obj.exprtk, sName, C.fun6_double(cFun))
	default:
		//C.addFunction(obj.exprtk,C.CString(funName),cFun)
		return
	}
	obj.funNameCache = append(obj.funNameCache, funName)
}

// 获取自动注册的变量列表
func (obj *GoExprtk) GetVariableList() []string {
	if len(obj.varListCache) < 1 {
		obj.autoCheckVariable()
	}
	return obj.varListCache
}

// 自动检测变量
func (obj *GoExprtk) autoCheckVariable() {
	str := obj.exprCache
	if len(str) < 1 {
		return
	}
	//特殊符号转换
	str = expressionFormat(str)      //变量格式化
	str += " "                       //
	hasVar := false                  //有变量申明 （有变量声明时的下一个变量不保存）
	varList := make(map[string]bool) //申明变量缓存

	start := 0 //首字符下标
	end := 1   //尾字符下标
	slen := len(str)
	for i := 0; i < slen; i++ {
		c := str[i]
		//fmt.Printf("%c\n",c)
		if isOperatorChar(c) || isWhiteSpace(c) { //操作符or空字符
			end = i
			if end > start {
				varStr := str[start:end]
				varStr = strings.TrimSpace(varStr)
				if varStr == "var" {
					hasVar = true
				} else if !isDigitString(varStr) &&
					!obj.isUserFunction(varStr) &&
					!isReservedSymbols(varStr) &&
					!isConstStr(varStr) { //排除数字字符串、保留字符（逻辑操作符、内建函数、分支控制结构词）、自定义函数、常量字符串（单引号修饰）

					varStr = strings.TrimLeftFunc(varStr, func(r rune) bool {
						//去掉字符串开头的数字，变量不会以数字开头，以兼容乘号省略的写法：2x+15x^2
						return isDigit(byte(r))
					})

					if hasVar {
						//hasVar 为真时说明这是申明的变量应该跳过
						hasVar = false
						varList[varStr] = true
					} else if _, ok := varList[varStr]; !ok {
						obj.cacheVariable(varStr) //缓存变量到obj列表
						//obj.AddDoubleVariable(varStr,0)
					}

				}
			}
			start = i + 1
		}
	}
}

// 添加变量到列表缓存
func (obj *GoExprtk) cacheVariable(varStr string) {
	for _, v := range obj.varListCache {
		if v == varStr {
			return
		}
	}
	varStr = expressionRestore(varStr) //格式化变量还原
	obj.varListCache = append(obj.varListCache, varStr)
}

// 是自定义函数
func (obj GoExprtk) isUserFunction(str string) bool {
	str = strings.ToLower(str)
	sFun := C.CString(str)
	defer C.free(unsafe.Pointer(sFun))

	if C.isUserFunction(obj.exprtk, sFun) == 1 {
		return true
	}

	for _, v := range obj.funNameCache {
		if strings.Compare(str, v) == 0 {
			return true
		}
	}
	return false
}

func Version() string {
	return C.GoString(C.version())
}
