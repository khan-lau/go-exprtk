package main

import (
	"fmt"

	"github.com/khan-lau/go-exprtk"
)

func example01() {
	// Create a new exprtk instance
	exprtkObj := exprtk.NewExprtk()

	// Set the expression
	exprtkObj.SetExpression("(x + 2)*(y-2)")

	// Add variables of expression
	exprtkObj.AddDoubleVariable("x", 1)
	exprtkObj.AddDoubleVariable("y", 2)

	// Compile the expression after expression and variables declaration
	err := exprtkObj.CompileExpression()
	if err != nil {
		fmt.Println(err.Error())
		return
	}

	// Set values for the variables
	exprtkObj.UpdateDoubleVariableValue("x", 18)
	exprtkObj.UpdateDoubleVariableValue("y", 32)

	// Get the evaluated value
	fmt.Println(exprtkObj.GetEvaluatedValue())

	// Modify values for the variables
	exprtkObj.UpdateDoubleVariableValue("y", 42)

	// Get re-evaluated value
	fmt.Println(exprtkObj.GetEvaluatedValue())
}
