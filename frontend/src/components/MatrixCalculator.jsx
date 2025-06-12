import { useState, useEffect } from 'react';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select";
import { AlertCircle } from "lucide-react";
import { Alert, AlertDescription, AlertTitle } from '@/components/ui/alert';
import { useApi } from '../hooks/useApi';

export const MatrixCalculator = () => {
  const [matrixA, setMatrixA] = useState('');
  const [matrixB, setMatrixB] = useState('');
  const [method, setMethod] = useState('auto');
  const [result, setResult] = useState(null);
  const [matrixSize, setMatrixSize] = useState(2);
  const { callApi, loading, error } = useApi();

  const handleCalculate = async () => {
    try {
      // Parse matrices from text
      const parseMatrix = (text) => {
        return text.trim().split('\n').map(row => 
          row.trim().split(/\s+/).map(num => parseInt(num, 10))
        );
      };
      
      const matrixAData = parseMatrix(matrixA);
      const matrixBData = parseMatrix(matrixB);
      
      // Validate matrices
      if (!validateMatrix(matrixAData) || !validateMatrix(matrixBData)) {
        throw new Error('Invalid matrix format');
      }
      
      // Send API request
      const data = {
        a: matrixAData,
        b: matrixBData,
        method
      };
      
      const response = await callApi('/matrix', 'POST', data);
      if (response) {
        setResult(response);
      }
    } catch (err) {
      console.error(err);
    }
  };
  
  // Validate that matrix is well-formed
  const validateMatrix = (matrix) => {
    if (!matrix || matrix.length === 0) return false;
    
    const rowLength = matrix[0].length;
    return matrix.every(row => row.length === rowLength && row.every(num => !isNaN(num)));
  };
  
  // Generate placeholder matrix
  const generatePlaceholder = (size) => {
    return Array(size).fill(0).map((_, i) => 
      Array(size).fill(0).map((_, j) => i * size + j + 1).join(' ')
    ).join('\n');
  };
  
  // Handle matrix size change
  const handleSizeChange = (newSize) => {
    setMatrixSize(newSize);
    setMatrixA(generatePlaceholder(newSize));
    setMatrixB(generatePlaceholder(newSize));
    setResult(null);
  };
  
  // Format matrix for display
  const formatMatrix = (matrix) => {
    if (!matrix) return '';
    return matrix.map(row => row.join(' ')).join('\n');
  };
  
  useEffect(() => {
    // Initialize with default matrices
    if (!matrixA && !matrixB) {
      handleSizeChange(2);
    }
  }, []);

  return (
    <Card>
      <CardHeader>
        <CardTitle>Matrix Multiplication</CardTitle>
        <CardDescription>
          Multiply matrices using Vedic mathematics
        </CardDescription>
      </CardHeader>
      <CardContent>
        <div className="grid gap-4">
          <div>
            <label className="text-sm font-medium">Matrix Size</label>
            <Select value={matrixSize.toString()} onValueChange={(val) => handleSizeChange(parseInt(val, 10))}>
              <SelectTrigger>
                <SelectValue placeholder="Select size" />
              </SelectTrigger>
              <SelectContent>
                <SelectItem value="2">2x2</SelectItem>
                <SelectItem value="3">3x3</SelectItem>
                <SelectItem value="4">4x4</SelectItem>
                <SelectItem value="8">8x8</SelectItem>
              </SelectContent>
            </Select>
          </div>
          
          <div className="grid grid-cols-2 gap-4">
            <div>
              <label className="text-sm font-medium">Matrix A</label>
              <textarea
                className="w-full h-32 p-2 border rounded-md font-mono text-sm"
                value={matrixA}
                onChange={(e) => setMatrixA(e.target.value)}
                placeholder="Enter matrix A (space-separated values, one row per line)"
              />
            </div>
            <div>
              <label className="text-sm font-medium">Matrix B</label>
              <textarea
                className="w-full h-32 p-2 border rounded-md font-mono text-sm"
                value={matrixB}
                onChange={(e) => setMatrixB(e.target.value)}
                placeholder="Enter matrix B (space-separated values, one row per line)"
              />
            </div>
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
            onClick={handleCalculate} 
            disabled={loading}
            className="w-full"
          >
            {loading ? "Calculating..." : "Multiply Matrices"}
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
              <h3 className="font-medium">Result Matrix</h3>
              <pre className="mt-2 p-2 bg-white border rounded-md font-mono text-sm overflow-auto">
                {formatMatrix(result.result)}
              </pre>
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

export default MatrixCalculator;