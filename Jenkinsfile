pipeline {
    agent any

    triggers {
        pollSCM('H/5 * * * *')
    }

    stages {
        stage('Clean before release') {
            steps {
                echo 'Cleaning'
                sh 'cd build; rm -rf *'
            }
        }

        stage('Release configure') {
            steps {
                echo 'Release configuring..'
                sh 'cd build; cmake -DCMAKE_BUILD_TYPE=Release ..'
            }
        }

        stage('Release build') {
            steps {
                echo 'Release building..'
                sh 'cd build; cmake --build . --config Release'
            }
        }

        stage('Clean after release') {
            steps {
                echo 'Cleaning'
                sh 'cd build; rm -rf *'
            }
        }

        stage('Debug configure') {
            steps {
                echo 'Debug configuring..'
                sh 'cd build; cmake -DCMAKE_BUILD_TYPE=Debug ..'
            }
        }

        stage('Debug build') {
            steps {
                echo 'Debug building..'
                sh 'cd build; cmake --build . --config Debug'
            }
        }
    }

    post
    {
        failure {
            slackSend message: "FAILED ${currentBuild.fullDisplayName}", color: "danger"
        }
        success {
            slackSend message: "SUCCEEDED ${currentBuild.fullDisplayName}", color: "good"
        }
    }
}
