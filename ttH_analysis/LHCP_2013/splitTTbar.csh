

foreach time (`seq 1 18`)

echo $time
head -n $time ttbar.list | tail -n 1 >&! ttbar_part${time}.list

end
