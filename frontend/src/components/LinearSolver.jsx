import { useState, useEffect } from 'react';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { Input } from "@/components/ui/input";
import { Button } from "@/components/ui/button";
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select";
import { AlertCircle } from "lucide-react";
import { Alert, AlertDescription, AlertTitle } from '@/components/ui/alert';
import { useApi } from '../hooks/useApi';

export const LinearSolver = () => {
  const [coefficients, setCoefficients] = useState('');
  const [constants, setConstants] = useState('');
  const [method, setMethod] = useState('auto');
  const [result, setResult] = useState(null);
  const [systemSize, setSystemSize] = useState(2);
  const { callApi, loading, error } = useApi();

  const handleSolve = async () => {
    try {
      // Parse coefficients and constants
      const parseMatrix = (text) => {
        return text.trim().split('\n').map(row => 
          row.trim().split(/\s+/).map(num => parseInt(num, 10))
        );
      };
      
      const coeffMatrix = parseMatrix(coefficients);
      const constArray = constants.trim().split(/\s+/).map(num => parseInt(num, 10));
      
      // Validate input
      if (!validateMatrix(coeffMatrix) || constArray.length !== coeffMatrix.length) {
        throw new Error('Invalid input format');
      }
      
      // Send API request
      const data = {
        coefficients: coeffMatrix,
        constants: constArray,
        method
      };
      
      const response = await callApi('/linear-solve', 'POST', data);
      if (response) {
        setResult(response);
      }
    } catch (err) {
      console.error(err);
    }
  };
  
  // Validate matrix format
  const validateMatrix = (matrix) => {
    if (!matrix || matrix.length === 0) return false;
    
    const rowLength = matrix[0].length;
    return matrix.every(row => row.length === rowLength && row.every(num => !isNaN(num)));
  };
  
  // Generate placeholder data
  const generatePlaceholders = (size) => {
    const coeffs = Array(size).fill(0).map((_, i) => 
      Array(size).fill(0).map((_, j) => i === j ? 1 : (j + 1)).join(' ')
    ).join('\n');
    
    const consts = Array(size).fill(0).map((_, i) => i + 1).join(' ');
    
    return { coeffs, consts };
  };
  
  // Handle system size change
  const handleSizeChange = (newSize) => {
    setSystemSize(newSize);
    const { coeffs, consts } = generatePlaceholders(newSize);
    setCoefficients(coeffs);
    setConstants(consts);
    setResult(null);
  };
  
  useEffect(() => {
    // Initialize with default values
    if (!coefficients && !constants) {
      handleSizeChange(2);
    }
  }, []);

  return (
    <Card>
      <CardHeader>
        <CardTitle>Linear Equation Solver</CardTitle>
        <CardDescription>
          Solve systems of linear equations using Vedic mathematics
        </CardDescription>
      </CardHeader>
      <CardContent>
        <div className="grid gap-4">
          <div>
            <label className="text-sm font-medium">System Size</label>
            <Select value={systemSize.toString()} onValueChange={(val) => handleSizeChange(parseInt(val, 10))}>
              <SelectTrigger>
                <SelectValue placeholder="Select size" />
              </SelectTrigger>
              <SelectContent>
                <SelectItem value="2">2x2</SelectItem>
                <SelectItem value="3">3x3</SelectItem>
                <SelectItem value="4">4x4</SelectItem>
              </SelectContent>
            </Select>
          </div>
          
          <div>
            <label className="text-sm font-medium">Coefficient Matrix</label>
            <textarea
              className="w-full h-32 p-2 border rounded-md font-mono text-sm"
              value={coefficients}
              onChange={(e) => setCoefficients(e.target.value)}
              placeholder="Enter coefficient matrix (space-separated values, one row per line)"
            />
          </div>
          
          <div>
            <label className="text-sm font-medium">Constants Vector</label>
            <Input 
              value={constants} 
              onChange={(e) => setConstants(e.target.value)} 
              placeholder="Enter constants (space-separated values)"
            />
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
          
          <Button 
            onClick={handleSolve} 
            disabled={loading}
            className="w-full"
          >
            {loading ? "Solving..." : "Solve System"}
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
              <h3 className="font-medium">Solution</h3>
              
              {result.has_solution ? (
                <div className="mt-2">
                  {result.solution.map((val, idx) => (
                    <div key={idx} className="py-1">
                      <span className="font-medium">x<sub>{idx+1}</sub> = </span>
                      <span>{val.toFixed(4)}</span>
                    </div>
                  ))}
                </div>
              ) : (
                <Alert className="mt-2">
                  <AlertCircle className="h-4 w-4" />
                  <AlertTitle>No Solution</AlertTitle>
                  <AlertDescription>This system has no unique solution.</AlertDescription>
                </Alert>
              )}
              
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

export default LinearSolver;