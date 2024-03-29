package main

import (
	"fmt"
	"math"

	"github.com/khan-lau/go-exprtk"
)

func example02() {
	var eqn string
	eqn = "if (eqn == 'avg') avg(x); "
	eqn += "else if (eqn == 'max') max(x); "
	eqn += "else if (eqn == 'min') min(x); "
	eqn += "else if (eqn == 'sum') sum(x); "
	eqn += "else 0; "

	var eqnStr string
	var array []float64 = []float64{1, 2, 3, -4.3, 10, -6.5, 7, 8, -1.3}

	exprtkObj := exprtk.NewExprtk()

	exprtkObj.SetExpression(eqn)
	exprtkObj.AddStringVariable("eqn", "max")
	exprtkObj.AddVectorVariable("x", []float64{1, 5, 2, 4.2, 10, 6.5, 7, 8, 1.3})
	exprtkObj.CompileExpression()
	exprtkObj.UpdateVectorVariableValue("x", array)

	eqnStr = "avg"
	exprtkObj.UpdateStringVariableValue("eqn", eqnStr)
	fmt.Println(math.Round(exprtkObj.GetEvaluatedValue()*100) / 100)

	eqnStr = "max"
	exprtkObj.UpdateStringVariableValue("eqn", eqnStr)
	fmt.Println(exprtkObj.GetEvaluatedValue())

	eqnStr = "min"
	exprtkObj.UpdateStringVariableValue("eqn", eqnStr)
	fmt.Println(exprtkObj.GetEvaluatedValue())

	eqnStr = "sum"
	exprtkObj.UpdateStringVariableValue("eqn", eqnStr)
	fmt.Println(exprtkObj.GetEvaluatedValue())

	eqnStr = "xyz"
	exprtkObj.UpdateStringVariableValue("eqn", eqnStr)
	fmt.Println(exprtkObj.GetEvaluatedValue())
}
