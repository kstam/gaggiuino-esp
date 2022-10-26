const ShotChartConfg = {
  animation: false,
  responsive: true,
  interaction: {
    mode: 'index',
    intersect: false,
  },
  stacked: false,
  plugins: {
    legend: {
      display: true,
      position: 'bottom',
    },
  },
  datasets: {
    line: {
      pointRadius: 0, // disable for all `'line'` datasets
    },
  },
  legend: {
    labels: {
      fontColor: '#ffffffc8',
    },
  },
  scales: {
    y1: {
      type: 'linear',
      display: true,
      position: 'left',
      suggestedMin: 0,
      suggestedMax: 100,
    },
    y2: {
      type: 'linear',
      display: true,
      position: 'right',
      suggestedMin: 0,
      suggestedMax: 12,
    },
  },
};

export default ShotChartConfg;
