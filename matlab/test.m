fp = fopen('3d_3_6.tns', 'r');
X = spTensor.load(1, fp);
fclose(fp);
fp = fopen('u.tns', 'r');
U = spTensor.load(1, fp).toMatrix();
fclose(fp);
Y = X.timesMatrix(U, 1);
Y = spTensor.fromSspTensor(Y);
fp = fopen('y.tns', 'w');
Y.dump(1, fp);
fclose(fp);
