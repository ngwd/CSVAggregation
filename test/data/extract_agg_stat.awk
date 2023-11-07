#!/usr/bin/awk -f
BEGIN {
  FS=","
  print "Zip Code, Date, Min Fine, Max Fine, Avg Fine, Count of Tickets"
}

NR > 1 {	# skip the header
  zip = $13
  date = substr($2, 0, 10)
  fine = $8
  # print zip ", " date ", " fine

  if (!min[zip, date] || fine < min[zip, date]) {
    min[zip, date] = fine
  }

  if (!max[zip, date] || fine > max[zip, date]) {
    max[zip, date] = fine
  }

  total[zip, date] += fine
  count[zip, date] ++
}

END {
  for (key in min) {
    split(key, parts, SUBSEP)
    zip = parts[1]
    date = parts[2]
    avg = total[key] / count[key]
    result[zip, date] = zip ", " date ", " min[key] ".0, " max[key] ".0, " sprintf("%.1f", avg) ", " count[key] ".0"
  }

  PROCINFO["sorted_in"] = "@ind_str_asc"
  for (key in result) {
    print result[key]
  }
}
