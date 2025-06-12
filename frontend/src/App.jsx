import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs";
import { ArithmeticCalculator } from './components/ArithmeticCalculator';
import { MatrixCalculator } from './components/MatrixCalculator';
import { LinearSolver } from './components/LinearSolver';
import { BenchmarkViewer } from './components/BenchmarkViewer';

// Main App Component
const App = () => {
  return (
    <div className="container mx-auto py-8 px-4">
      <div className="text-center mb-8">
        <h1 className="text-3xl font-bold">VedicMathEngine</h1>
        <p className="text-gray-600 mt-2">
          Adaptive arithmetic computation engine leveraging Vedic Mathematics
        </p>
      </div>
      
      <Tabs defaultValue="arithmetic" className="w-full">
        <TabsList className="grid grid-cols-4 mb-8">
          <TabsTrigger value="arithmetic">Arithmetic</TabsTrigger>
          <TabsTrigger value="matrix">Matrix</TabsTrigger>
          <TabsTrigger value="linear">Linear Equations</TabsTrigger>
          <TabsTrigger value="benchmark">Benchmarks</TabsTrigger>
        </TabsList>
        
        <TabsContent value="arithmetic">
          <ArithmeticCalculator />
        </TabsContent>
        
        <TabsContent value="matrix">
          <MatrixCalculator />
        </TabsContent>
        
        <TabsContent value="linear">
          <LinearSolver />
        </TabsContent>
        
        <TabsContent value="benchmark">
          <BenchmarkViewer />
        </TabsContent>
      </Tabs>
    </div>
  );
};

export default App;