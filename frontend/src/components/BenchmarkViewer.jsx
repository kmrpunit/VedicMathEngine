import { useState, useEffect } from 'react';
import { Card, CardContent, CardDescription, CardFooter, CardHeader, CardTitle } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { AlertCircle, BarChart4 } from "lucide-react";
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer } from 'recharts';
import { Alert, AlertDescription, AlertTitle } from '@/components/ui/alert';
import { useApi } from '../hooks/useApi';

const API_URL = "http://localhost:8000";

export const BenchmarkViewer = () => {
  const [benchmarkData, setBenchmarkData] = useState([]);
  const [chartData, setChartData] = useState([]);
  const { callApi, loading, error } = useApi();
  
  const fetchBenchmarkData = async () => {
    const response = await callApi('/benchmark');
    if (response && response.data) {
      setBenchmarkData(response.data);
      
      // Prepare chart data
      const chartData = response.data
        .filter(item => item.Operation === "Multiplication")
        .map(item => ({
          operation: item.Operation,
          input: `${item.Input1}×${item.Input2}`,
          standard: item.MethodUsed === "Standard" ? parseFloat(item.TimeTaken) : null,
          vedic: item.MethodUsed === "Vedic" ? parseFloat(item.TimeTaken) : null,
        }));
      
      setChartData(chartData);
    }
  };
  
  useEffect(() => {
    fetchBenchmarkData();
  }, []);
  
  const downloadBenchmark = () => {
    window.open(`${API_URL}/benchmark/download`, '_blank');
  };

  return (
    <Card>
      <CardHeader>
        <CardTitle className="flex items-center gap-2">
          <BarChart4 className="h-5 w-5" />
          Benchmark Results
        </CardTitle>
        <CardDescription>
          Performance comparison between standard and Vedic methods
        </CardDescription>
      </CardHeader>
      <CardContent>
        {loading ? (
          <div className="text-center py-4">Loading benchmark data...</div>
        ) : error ? (
          <Alert variant="destructive">
            <AlertCircle className="h-4 w-4" />
            <AlertTitle>Error</AlertTitle>
            <AlertDescription>{error}</AlertDescription>
          </Alert>
        ) : benchmarkData.length === 0 ? (
          <div className="text-center py-4">No benchmark data available yet. Perform some calculations first.</div>
        ) : (
          <div className="grid gap-6">
            <div className="h-80">
              <h3 className="text-sm font-medium mb-2">Performance Comparison</h3>
              <ResponsiveContainer width="100%" height="100%">
                <LineChart
                  data={chartData}
                  margin={{ top: 5, right: 30, left: 20, bottom: 5 }}
                >
                  <CartesianGrid strokeDasharray="3 3" />
                  <XAxis dataKey="input" />
                  <YAxis label={{ value: 'Time (ms)', angle: -90, position: 'insideLeft' }} />
                  <Tooltip />
                  <Legend />
                  <Line 
                    type="monotone" 
                    dataKey="standard" 
                    name="Standard" 
                    stroke="#8884d8" 
                    activeDot={{ r: 8 }} 
                  />
                  <Line 
                    type="monotone" 
                    dataKey="vedic" 
                    name="Vedic" 
                    stroke="#82ca9d" 
                  />
                </LineChart>
              </ResponsiveContainer>
            </div>
            
            <div>
              <div className="flex justify-between items-center mb-2">
                <h3 className="text-sm font-medium">Recent Benchmark Results</h3>
                <Button onClick={downloadBenchmark} variant="outline" size="sm">
                  Download CSV
                </Button>
              </div>
              <div className="border rounded-md overflow-hidden">
                <table className="min-w-full divide-y divide-gray-200">
                  <thead className="bg-gray-50">
                    <tr>
                      <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Operation</th>
                      <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Inputs</th>
                      <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Method</th>
                      <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Time (ms)</th>
                    </tr>
                  </thead>
                  <tbody className="bg-white divide-y divide-gray-200">
                    {benchmarkData.slice(0, 10).map((item, index) => (
                      <tr key={index}>
                        <td className="px-6 py-4 whitespace-nowrap text-sm">{item.Operation}</td>
                        <td className="px-6 py-4 whitespace-nowrap text-sm font-mono">
                          {item.Operation === "Multiplication" ? 
                            `${item.Input1} × ${item.Input2}` : 
                            `${item.Input1}, ${item.Input2}`}
                        </td>
                        <td className="px-6 py-4 whitespace-nowrap text-sm">
                          <span className={`px-2 py-1 rounded-full text-xs ${
                            item.MethodUsed === "Vedic" ? "bg-green-100 text-green-800" : "bg-blue-100 text-blue-800"
                          }`}>
                            {item.MethodUsed}
                          </span>
                        </td>
                        <td className="px-6 py-4 whitespace-nowrap text-sm">{parseFloat(item.TimeTaken).toFixed(6)}</td>
                      </tr>
                    ))}
                  </tbody>
                </table>
              </div>
            </div>
          </div>
        )}
      </CardContent>
      <CardFooter>
        <Button onClick={fetchBenchmarkData} variant="outline" className="w-full">
          Refresh Benchmark Data
        </Button>
      </CardFooter>
    </Card>
  );
};

export default BenchmarkViewer;