BEGIN {
  N = 65
  delay = 33
  outfile = "rbt_anim.gif"

  lmax = 0
  rmax = 0
  hmax = 0
  for (i=0; i<=N; i++) {
    name = sprintf("rbt_%03d", i)
#    system(sprintf("sips -s format bmp %s.gif --out %s.bmp > /dev/null", name, name))
    system(sprintf("convert %s.gif %s.bmp", name, name))
    cmd = sprintf("./topcenter %s.bmp", name)
    cmd | getline
    if ($3 < 0) $3 = 0
    if ($4 < 0) $4 = 0
    w[i] = $1; h[i] = $2; x[i] = $3; y[i] = $4;
    if ($3 > lmax) lmax = $3;
    if ($1 - $3 > rmax) rmax = $1 - $3
    if ($2 > hmax) hmax = $2
    close(cmd)
    system(sprintf("rm -f %s.bmp", name))
  }

  cmd = sprintf("convert -delay %d -size %dx%d xc:White", delay, lmax + rmax, hmax)
  for (i=0; i<=N; i++) {
    cmd = cmd sprintf(" -page +%d+%d rbt_%03d.gif", lmax - x[i], 0, i)
  }
  cmd = cmd " -loop 0 " outfile

  system(cmd)
  exit;
}
