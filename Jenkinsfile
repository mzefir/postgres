pipeline {
  agent any

  stages {
    stage('Preparations') {
      steps {
        sh 'git submodule init'
        sh 'git submodule update'
        // sh 'docker stop gcc11 2> /dev/null || true'
        // sh 'docker stop gcc12 2> /dev/null || true'
        // sh 'docker stop gcc13 2> /dev/null || true'
        sh 'docker stop gcc14 2> /dev/null || true'
        sh 'sleep 10'
      }
    }
    stage('Parallel Stages') {
      parallel {
        // stage("GCC 11") {
        //   stages {
        //     stage('Start docker') {
        //       steps {
        //         sh 'docker pull sidobado/gcc11:latest'
        //         sh 'docker run --name gcc11 -e REPO_ROOT=/home/gcc/app/ --rm -dti -v $PWD:/home/gcc/app sidobado/gcc11:latest bash'
        //       }
        //     }
        //     stage('Build Dev Version') {
        //       steps {
        //         sh "docker exec -t --user 1000:1000 gcc11 bash -c 'mkdir -p /home/gcc/build11; cd /home/gcc/build11; cmake -DCMAKE_BUILD_TYPE=Debug /home/gcc/app/test'"
        //         sh "docker exec -t --user 1000:1000 gcc11 bash -c 'cd /home/gcc/build11; make'"
        //         sh "docker exec -t --user 1000:1000 gcc11 bash -c 'mkdir -p /home/gcc/examples11; cd /home/gcc/examples11; cmake -DCMAKE_BUILD_TYPE=Debug /home/gcc/app/examples'"
        //         sh "docker exec -t --user 1000:1000 gcc11 bash -c 'cd /home/gcc/examples11; make -j4'"
        //       }
        //     }
        //     stage('Execute tests') {
        //       steps {
        //         sh "docker exec -t --user 1000:1000 gcc11 bash -c 'cd /home/gcc/build11; rm -fr coverage*; make coverage -j4'"
        //         sh "docker exec -t --user 1000:1000 gcc11 bash -c 'cd /home/gcc/build11; mv coverage gcov11; tar -cf gcov11.tar gcov11;'"
        //         sh "docker exec -t --user 1000:1000 gcc11 bash -c 'mkdir -p out; mv /home/gcc/build11/gcov11.tar out/'"
        //         sh "docker exec -t --user 1000:1000 gcc11 bash -c 'cd out; tar -xf gcov11.tar; rm -f gcov11.tar'"
        //       }
        //     }
        //     // stage('Publish code coverage') {
        //     //   steps {
        //     //     publishHTML target: [
        //     //       allowMissing: false,
        //     //       alwaysLinkToLastBuild: false,
        //     //       keepAll: true,
        //     //       reportDir: 'out/gcov11',
        //     //       reportFiles: 'index.html',
        //     //       reportName: 'Code coverage (GCC 11)',
        //     //       reportTitles: 'Code coverage (GCC 11)',
        //     //       useWrapperFileDirectly: true
        //     //     ]
        //     //   }
        //     // }
        //     stage('Stop docker') {
        //       steps {
        //         sh 'docker stop gcc11 2> /dev/null || true'
        //       }
        //     }
        //   }
        // }
        // stage("GCC 12") {
        //   stages {
        //     stage('Start docker') {
        //       steps {
        //         sh 'docker pull sidobado/gcc12:latest'
        //         sh 'docker run --name gcc12 -e REPO_ROOT=/home/gcc/app/ --rm -dti -v $PWD:/home/gcc/app sidobado/gcc12:latest bash'
        //       }
        //     }
        //     stage('Build Dev Version') {
        //       steps {
        //         sh "docker exec -t --user 1000:1000 gcc12 bash -c 'mkdir -p /home/gcc/build12; cd /home/gcc/build12; cmake -DCMAKE_BUILD_TYPE=Debug /home/gcc/app/test'"
        //         sh "docker exec -t --user 1000:1000 gcc12 bash -c 'cd /home/gcc/build12; make -j4'"
        //         sh "docker exec -t --user 1000:1000 gcc14 bash -c 'mkdir -p /home/gcc/examples12; cd /home/gcc/examples12; cmake -DCMAKE_BUILD_TYPE=Debug /home/gcc/app/examples'"
        //         sh "docker exec -t --user 1000:1000 gcc14 bash -c 'cd /home/gcc/examples12; make -j4'"
        //       }
        //     }
        //     stage('Execute tests') {
        //       steps {
        //         sh "docker exec -t --user 1000:1000 gcc12 bash -c 'cd /home/gcc/build12; rm -fr coverage*; make coverage -j4'"
        //         sh "docker exec -t --user 1000:1000 gcc12 bash -c 'cd /home/gcc/build12; mv coverage gcov12; tar -cf gcov12.tar gcov12;'"
        //         sh "docker exec -t --user 1000:1000 gcc12 bash -c 'mkdir -p out; mv /home/gcc/build12/gcov12.tar out/'"
        //         sh "docker exec -t --user 1000:1000 gcc12 bash -c 'cd out; tar -xf gcov12.tar; rm -f gcov12.tar'"
        //       }
        //     }
        //     // stage('Publish code coverage') {
        //     //   steps {
        //     //     publishHTML target: [
        //     //       allowMissing: false,
        //     //       alwaysLinkToLastBuild: false,
        //     //       keepAll: true,
        //     //       reportDir: 'out/gcov12',
        //     //       reportFiles: 'index.html',
        //     //       reportName: 'Code coverage (GCC 12)',
        //     //       reportTitles: 'Code coverage (GCC 12)',
        //     //       useWrapperFileDirectly: true
        //     //     ]
        //     //   }
        //     // }
        //     stage('Stop docker') {
        //       steps {
        //         sh 'docker stop gcc12 2> /dev/null || true'
        //       }
        //     }
        //   }
        // }
        // stage("GCC 13") {
        //   stages {
        //     stage('Start docker') {
        //       steps {
        //         sh 'docker pull sidobado/gcc13:latest'
        //         sh 'docker run --name gcc13 -e REPO_ROOT=/home/gcc/app/ --rm -dti -v $PWD:/home/gcc/app sidobado/gcc13:latest bash'
        //       }
        //     }
        //     stage('Build Dev Version') {
        //       steps {
        //         sh "docker exec -t --user 1000:1000 gcc13 bash -c 'mkdir -p /home/gcc/build13; cd /home/gcc/build13; cmake -DCMAKE_BUILD_TYPE=Debug /home/gcc/app/test'"
        //         sh "docker exec -t --user 1000:1000 gcc13 bash -c 'cd /home/gcc/build13; make -j4'"
        //         sh "docker exec -t --user 1000:1000 gcc14 bash -c 'mkdir -p /home/gcc/examples13; cd /home/gcc/examples13; cmake -DCMAKE_BUILD_TYPE=Debug /home/gcc/app/examples'"
        //         sh "docker exec -t --user 1000:1000 gcc14 bash -c 'cd /home/gcc/examples13; make -j4'"
        //       }
        //     }
        //     stage('Execute tests') {
        //       steps {
        //         sh "docker exec -t --user 1000:1000 gcc13 bash -c 'cd /home/gcc/build13; rm -fr coverage*; make coverage -j4'"
        //         sh "docker exec -t --user 1000:1000 gcc13 bash -c 'cd /home/gcc/build13; mv coverage gcov13; tar -cf gcov13.tar gcov13;'"
        //         sh "docker exec -t --user 1000:1000 gcc13 bash -c 'mkdir -p out; mv /home/gcc/build13/gcov13.tar out/'"
        //         sh "docker exec -t --user 1000:1000 gcc13 bash -c 'cd out; tar -xf gcov13.tar; rm -f gcov13.tar'"
        //       }
        //     }
        //     // stage('Publish code coverage') {
        //     //   steps {
        //     //     publishHTML target: [
        //     //       allowMissing: false,
        //     //       alwaysLinkToLastBuild: false,
        //     //       keepAll: true,
        //     //       reportDir: 'out/gcov13',
        //     //       reportFiles: 'index.html',
        //     //       reportName: 'Code coverage (GCC 13)',
        //     //       reportTitles: 'Code coverage (GCC 13)',
        //     //       useWrapperFileDirectly: true
        //     //     ]
        //     //   }
        //     // }
        //     // stage('Execute CodeChecker') {
        //     //   steps {
        //     //     sh "docker exec -t --user 1000:1000 gcc13 bash -c 'cd /home/gcc/build13; CodeChecker check -b \"make clean && make\" -i /home/gcc/app/codechecker_skip.txt -o ./codechecker_results' || true"
        //     //     sh "docker exec -t --user 1000:1000 gcc13 bash -c 'cd /home/gcc/build13; CodeChecker parse -e html ./codechecker_results -o ./codechecker13_html' || true"
        //     //     sh "docker exec -t --user 1000:1000 gcc13 bash -c 'cd /home/gcc/build13; tar -cf codechecker13.tar codechecker13_html;'"
        //     //     sh "docker exec -t --user 1000:1000 gcc13 bash -c 'mkdir -p out; mv /home/gcc/build13/codechecker13.tar out/'"
        //     //     sh "docker exec -t --user 1000:1000 gcc13 bash -c 'cd out; tar -xf codechecker13.tar; rm -f codechecker13.tar'"
        //     //   }
        //     // }
        //     // stage('Publish CodeChecker') {
        //     //   steps {
        //     //     publishHTML target: [
        //     //       allowMissing: false,
        //     //       alwaysLinkToLastBuild: false,
        //     //       keepAll: true,
        //     //       reportDir: 'out/codechecker13_html',
        //     //       reportFiles: 'index.html',
        //     //       reportName: 'Code Checker (GCC 13)',
        //     //       reportTitles: 'Code Checker (GCC 13)',
        //     //       useWrapperFileDirectly: true
        //     //     ]
        //     //   }
        //     // }
        //     stage('Stop docker') {
        //       steps {
        //         sh 'docker stop gcc13 2> /dev/null || true'
        //       }
        //     }
        //   }
        // }
        stage("GCC 14") {
          stages {
            stage('Start docker') {
              steps {
                sh 'docker pull sidobado/gcc14:latest'
                sh 'docker run --name gcc14 -e REPO_ROOT=/home/gcc/app/ --rm -dti -v $PWD:/home/gcc/app sidobado/gcc14:latest bash'
              }
            }
            stage('Build Dev Version') {
              steps {
                sh "docker exec -t --user 1000:1000 gcc14 bash -c 'mkdir -p /home/gcc/build14; cd /home/gcc/build14; cmake -DCMAKE_BUILD_TYPE=Debug /home/gcc/app'"
                sh "docker exec -t --user 1000:1000 gcc14 bash -c 'cd /home/gcc/build14; make -j4'"
                // sh "docker exec -t --user 1000:1000 gcc14 bash -c 'mkdir -p /home/gcc/examples14; cd /home/gcc/examples14; cmake -DCMAKE_BUILD_TYPE=Debug /home/gcc/app/examples'"
                // sh "docker exec -t --user 1000:1000 gcc14 bash -c 'cd /home/gcc/examples14; make -j4'"
              }
            }
            stage('Execute tests') {
              steps {
                sh "docker exec -t --user 1000:1000 gcc14 bash -c 'cd /home/gcc/build14; rm -fr coverage*; make coverage -j4'"
                sh "docker exec -t --user 1000:1000 gcc14 bash -c 'cd /home/gcc/build14; mv coverage gcov14; tar -cf gcov14.tar gcov14;'"
                sh "docker exec -t --user 1000:1000 gcc14 bash -c 'mkdir -p out; mv /home/gcc/build14/gcov14.tar out/'"
                sh "docker exec -t --user 1000:1000 gcc14 bash -c 'cd out; tar -xf gcov14.tar; rm -f gcov14.tar'"
                publishHTML target: [
                  allowMissing: false,
                  alwaysLinkToLastBuild: false,
                  keepAll: true,
                  reportDir: 'out/gcov14',
                  reportFiles: 'index.html',
                  reportName: 'Code coverage (GCC 14)',
                  reportTitles: 'Code coverage (GCC 14)',
                  useWrapperFileDirectly: true
                ]
              }
            }
            stage('Execute CodeChecker') {
              steps {
                sh "docker exec -t --user 1000:1000 gcc14 bash -c 'cd /home/gcc/build14; CodeChecker check -b \"make clean && make -j4\" -i /home/gcc/app/codechecker_skip.txt -o ./codechecker_results' || true"
                sh "docker exec -t --user 1000:1000 gcc14 bash -c 'cd /home/gcc/build14; CodeChecker parse -e html ./codechecker_results -o ./codechecker14_html' || true"
                sh "docker exec -t --user 1000:1000 gcc14 bash -c 'cd /home/gcc/build14; tar -cf codechecker14.tar codechecker14_html;'"
                sh "docker exec -t --user 1000:1000 gcc14 bash -c 'mkdir -p out; mv /home/gcc/build14/codechecker14.tar out/'"
                sh "docker exec -t --user 1000:1000 gcc14 bash -c 'cd out; tar -xf codechecker14.tar; rm -f codechecker14.tar'"
                publishHTML target: [
                  allowMissing: false,
                  alwaysLinkToLastBuild: false,
                  keepAll: true,
                  reportDir: 'out/codechecker14_html',
                  reportFiles: 'index.html',
                  reportName: 'Code Checker (GCC 14)',
                  reportTitles: 'Code Checker (GCC 14)',
                  useWrapperFileDirectly: true
                ]
              }
            }
            stage('Documentation') {
              steps {
                sh "docker exec -t --user 1000:1000 gcc14 bash -c 'rm -fr out/doc; mkdir -p out/doc'"
                sh "docker exec -t --user 1000:1000 gcc14 bash -c './utils/generate_docs.sh'"
                publishHTML target: [
                  allowMissing: false,
                  alwaysLinkToLastBuild: false,
                  keepAll: true,
                  reportDir: 'out/doc',
                  reportFiles: 'index.html',
                  reportName: 'Documentation',
                  reportTitles: 'Documentation',
                  useWrapperFileDirectly: true
                ]
              }
            }
            stage('Stop docker') {
              steps {
                sh 'docker stop gcc14 2> /dev/null || true'
              }
            }
          }
        }
      }
    }

    stage('Build Production') {
      steps {
        sh 'echo "Left for future usage"'
      }
    }
    stage('Deploy') {
      steps {
        sh 'echo "Left for future usage"'
      }
    }
    stage('Clean') {
      steps {
        // sh 'docker stop gcc11 2> /dev/null || true'
        // sh 'docker stop gcc12 2> /dev/null || true'
        // sh 'docker stop gcc13 2> /dev/null || true'
        sh 'docker stop gcc14 2> /dev/null || true'
      }
    }
  }
}
