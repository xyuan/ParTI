fp = fopen('a.tns', 'r');
X = spTensor.load(1, fp);
fclose(fp);
fp = fopen('b.tns', 'r');
U = spTensor.load(1, fp).toMatrix();
fclose(fp);
Y = X.timesMatrix(U, 2);
Y = spTensor.fromSspTensor(Y, 1e-6);
fp = fopen('y.tns', 'w');
Y.dump(1, fp);
fclose(fp);
