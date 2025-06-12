import { useState } from 'react';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { Input } from "@/components/ui/input";
import { Button } from "@/components/ui/button";
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select";
import { AlertCircle, Calculator, X, Plus, Minus, Divide } from "lucide-react";
import { Alert, AlertDescription, AlertTitle } from '@/components/ui/alert';
import { useApi } from '../hooks/useApi';

export const ArithmeticCalculator = () => {
  const [input1, setInput1] = useState('');
  const [input2, setInput2] = useState('');
  const [operation, setOperation] = useState('multiply');
  const [method, setMethod] = useState('auto');
  const [result, setResult] = useState(null);
  const { callApi, loading, error } = useApi();

  const handleCalculate = async () => {
    if (!input1 || !input2) return;

    const data = {
      a: parseInt(input1, 10),
      b: parseInt(input2, 10),
      operation,
      method
    };

    const response = await callApi('/calculate', 'POST', data);
    if (response) {
      setResult(response);
    }
  };

  const getOperationIcon = () => {
    switch (operation) {
      case 'add': return <Plus className="h-4 w-4" />;
      case 'subtract': return <Minus className="h-4 w-4" />;
      case 'multiply': return <X className="h-4 w-4" />;
      case 'divide': return <Divide className="h-4 w-4" />;
      default: return <Calculator className="h-4 w-4" />;
    }
  };

  return (
    <Card>
      <CardHeader>
        <CardTitle>Arithmetic Calculator</CardTitle>
        <CardDescription>
          Perform arithmetic operations using Vedic mathematics
        </CardDescription>
      </CardHeader>
      <CardContent>
        <div className="grid gap-4">
          <div className="grid grid-cols-2 gap-4">
            <div>
              <label className="text-sm font-medium">First Number</label>
              <Input 
                type="number" 
                value={input1} 
                onChange={(e) => setInput1(e.target.value)} 
                placeholder="Enter first number"
              />
            </div>
            <div>
              <label className="text-sm font-medium">Second Number</label>
              <Input 
                type="number" 
                value={input2} 
                onChange={(e) => setInput2(e.target.value)} 
                placeholder="Enter second number"
              />
            </div>
          </div>
          
          <div className="grid grid-cols-2 gap-4">
            <div>
              <label className="text-sm font-medium">Operation</label>
              <Select value={operation} onValueChange={setOperation}>
                <SelectTrigger>
                  <SelectValue placeholder="Select operation" />
                </SelectTrigger>
                <SelectContent>
                  <SelectItem value="add">Addition</SelectItem>
                  <SelectItem value="subtract">Subtraction</SelectItem>
                  <SelectItem value="multiply">Multiplication</SelectItem>
                  <SelectItem value="divide">Division</SelectItem>
                </SelectContent>
              </Select>
            </div>
            <div>
              <label className="text-sm font-medium">Method</label>
              <Select value={method} onValueChange={setMethod}>
                <SelectTrigger>
                  <SelectValue placeholder="Select method" />
                </SelectTrigger>
                <SelectContent>
                  <SelectItem value="auto">Auto (Adaptive)</SelectItem>
                  <SelectItem value="standard">Standard</SelectItem>
                  <SelectItem value="vedic">Vedic</SelectItem>
                </SelectContent>
              </Select>
            </div>
          </div>
          
          <Button 
            onClick={handleCalculate} 
            disabled={loading || !input1 || !input2}
            className="w-full"
          >
            {loading ? "Calculating..." : "Calculate"}
          </Button>
          
          {error && (
            <Alert variant="destructive">
              <AlertCircle className="h-4 w-4" />
              <AlertTitle>Error</AlertTitle>
              <AlertDescription>{error}</AlertDescription>
            </Alert>
          )}
          
          {result && (
            <div className="mt-4 p-4 border rounded-md bg-slate-50">
              <h3 className="font-medium flex items-center gap-2">
                {getOperationIcon()} Result
              </h3>
              <div className="text-3xl font-bold mt-2">{result.result}</div>
              <div className="mt-4 grid grid-cols-2 gap-2 text-sm">
                <div>
                  <span className="font-medium">Method Used:</span>{" "}
                  <span className="capitalize">{result.method_used}</span>
                </div>
                <div>
                  <span className="font-medium">Sutra:</span>{" "}
                  <span>{result.sutra_name}</span>
                </div>
                <div>
                  <span className="font-medium">Time Taken:</span>{" "}
                  <span>{result.time_taken.toFixed(6)} ms</span>
                </div>
                <div>
                  <span className="font-medium">Memory Used:</span>{" "}
                  <span>{result.memory_used} bytes</span>
                </div>
              </div>
            </div>
          )}
        </div>
      </CardContent>
    </Card>
  );
};

export default ArithmeticCalculator;