/^C /{w[$2]= $5};
END{
  for(i=0;i<256;i++)
  {
    if (length(w[i])==0)
      printf ("0, ");
   else
      printf ("%d, ", w[i]);
  }
}

