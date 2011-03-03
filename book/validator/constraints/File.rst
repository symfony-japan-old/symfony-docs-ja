File
====

.. Validates that a value is the path to an existing file.

値が存在するファイルのパスであることのバリデーションを実行します。

.. code-block:: yaml

    properties:
        filename:
            - File: ~

オプション
----------

.. * ``maxSize``: The maximum allowed file size. Can be provided in bytes, kilobytes
     (with the suffix "k") or megabytes (with the suffix "M")
   * ``mimeTypes``: One or more allowed mime types
   * ``notFoundMessage``: The error message if the file was not found
   * ``notReadableMessage``: The error message if the file could not be read
   * ``maxSizeMessage``: The error message if ``maxSize`` validation fails
   * ``mimeTypesMessage``: The error message if ``mimeTypes`` validation fails

* ``maxSize``: 許されている最大ファイルサイズ。バイト単位、キロバイト単位 (末尾に "k") 、メガバイト単位 (末尾に "M") で表せます。
* ``mimeTypes``: 許されている1つまたは複数の MIME タイプ。
* ``notFoundMessage``: ファイルが見つからない場合のエラーメッセージ。
* ``notReadableMessage``: ファイルが読み込みできない場合のエラーメッセージ。
* ``maxSizeMessage``: ``maxSize`` バリデーションが失敗した場合のエラーメッセージ
* ``mimeTypesMessage``: ``mimeTypes`` バリデーションが失敗した場合のエラーメッセージ

例: ファイルサイズと MIME タイプのバリデーションを行う
------------------------------------------------------

.. In this example we use the ``File`` constraint to verify that the file does
   not exceed a maximum size of 128 kilobytes and is a PDF document.

この例では、ファイルが 128KB の最大サイズを超えていないことと、PDF ドキュメントであることを検証するために、 ``File`` 制約を使います。

.. configuration-block::

    .. code-block:: yaml

        properties:
            filename:
                - File: { maxSize: 128k, mimeTypes: [application/pdf, application/x-pdf] }

    .. code-block:: xml

        <!-- Sensio/HelloBundle/Resources/config/validation.xml -->
        <class name="Sensio\HelloBundle\Author">
            <property name="filename">
                <constraint name="File">
                    <option name="maxSize">128k</option>
                    <option name="mimeTypes">
                        <value>application/pdf</value>
                        <value>application/x-pdf</value>
                    </option>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // Sensio/HelloBundle/Author.php
        class Author
        {
            /**
             * @validation:File(maxSize = "128k", mimeTypes = {
             *   "application/pdf",
             *   "application/x-pdf"
             * })
             */
            private $filename;
        }

    .. code-block:: php

        // Sensio/HelloBundle/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\File;

        class Author
        {
            private $filename;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('filename', new File(array(
                    'maxSize' => '128k',
                    'mimeTypes' => array(
                        'application/pdf',
                        'application/x-pdf',
                    ),
                )));
            }
        }

.. When you validate the object with a file that doesn't satisfy one of these
   constraints, a proper error message is returned by the validator:

これらの制約の1つを満たさないファイルを持つオブジェクトのバリデーションを行うと、しかるべきエラーメッセージがバリデータから返されます。

.. code-block:: text

    Sensio\HelloBundle\Author.filename:
        The file is too large (150 kB). Allowed maximum size is 128 kB
