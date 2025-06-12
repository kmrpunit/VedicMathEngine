import { useState } from 'react';

const API_URL = "http://localhost:8000";

// Custom hook for API calls
export const useApi = () => {
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);

  const callApi = async (endpoint, method = 'GET', data = null) => {
    setLoading(true);
    setError(null);

    try {
      const options = {
        method,
        headers: {
          'Content-Type': 'application/json',
        },
        body: data ? JSON.stringify(data) : undefined,
      };

      const response = await fetch(`${API_URL}${endpoint}`, options);
      
      if (!response.ok) {
        const errorData = await response.json();
        throw new Error(errorData.detail || 'API request failed');
      }

      const result = await response.json();
      return result;
    } catch (err) {
      setError(err.message);
      return null;
    } finally {
      setLoading(false);
    }
  };

  return { callApi, loading, error };
};

export default useApi;